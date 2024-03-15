//
//  GameManager.cpp
//  Server
//
//  Created by Fedor Fedor on 07.04.2023.
//

#include <memory>
#include <chrono>
#include <thread>

#include "GameManager.hpp"
#include "sys/InetNetworker.hpp"

namespace engine
{

GameManager::GameManager()
{
    // Создаём корневой объект сервера
    m_server_root = std::make_shared<Object>(server_root_id, -1, this);
    m_id_to_object[server_root_id] = m_server_root;
    m_objects.push_back(m_server_root);
    
    // Создаём корневой объект клиента
    m_client_root = std::make_shared<Object>(client_root_id, -1, this);
    m_id_to_object[client_root_id] = m_client_root;
    m_objects.push_back(m_client_root);
    
    // Устанавливаем максимальный использованный уникальный номер
    m_max_id = client_root_id;

    // Создаём экземпляр сетевого модуля
    m_networker = std::make_unique<sys::InetNetworker>();

    // Создаём экземпляр менеджера компонентов
    m_component_manager = std::make_shared<ComponentManager>();

    // Создаём экземпляр контроллера
    m_controller = std::make_shared<Controller>();

    // Создаём экземпляр клавиатуры
    m_keyboard = std::make_shared<Keyboard>(m_controller);
    
    m_running = false;
}

std::shared_ptr<Object> GameManager::get_server_root()
{
    return m_server_root;
}

std::shared_ptr<Object> GameManager::get_client_root()
{
    return m_client_root;
}

std::shared_ptr<Object> GameManager::get_root()
{
    return m_client_root;
}

std::shared_ptr<Object> GameManager::add_object(long long parent_id)
{
    if(m_id_to_object.find(parent_id) == m_id_to_object.end())
    {
        error("[GameManager::add_object(" + std::to_string(parent_id) + ")] Object with given parent ID does not exist.");
        return nullptr;
    }

    if(parent_id < client_root_id)
    {
        error("[GameManager::add_object(" + std::to_string(parent_id) + ")] Cannot add an object to the server tree.");
        return nullptr;
    }

    m_max_id += 1;
    auto new_object = std::make_shared<Object>(m_max_id, parent_id, this);
    m_id_to_object[m_max_id] = new_object;
    m_id_to_object[parent_id]->register_child(new_object);
    m_objects.push_back(new_object);
    
    return new_object;
}

std::shared_ptr<Object> GameManager::add_server_object()
{
    return add_object(server_root_id);
}

std::shared_ptr<Object> GameManager::add_client_object()
{
    return add_object(client_root_id);
}

std::shared_ptr<Object> GameManager::add_object(long long parent_id, std::string name)
{
    auto new_object = add_object(parent_id);
    new_object->set_name(name);
    
    return new_object;
}

std::shared_ptr<Object> GameManager::add_client_object(std::string name)
{
    return add_object(client_root_id, name);
}

void GameManager::unregister_object(long long object_id)
{
    m_id_to_object.erase(object_id);

    for(long long i = 0; i < m_objects.size(); i++)
    {
        if(m_objects[i]->get_id() == object_id)
        {
            m_objects.erase(m_objects.begin() + i);
            break;
        }
    }
}

std::shared_ptr<Object> GameManager::get_object(long long id)
{
    if(m_id_to_object.find(id) == m_id_to_object.end())
    {
        // warning("[GameManager::get_object] Object with given ID was not found.");
        return nullptr;
    } else {
        return m_id_to_object[id];
    }
}

std::vector<std::shared_ptr<Object>> GameManager::get_objects()
{
    return m_objects;
}

void GameManager::add_system(std::shared_ptr<ISystem> system)
{
    if(m_running)
    {
        error("[GameManager::add_system()] Unable to add a system after start.");
        return;
    }
    m_systems.push_back(system);
}

void GameManager::connect(std::string address)
{
    m_networker->connect(address);
    m_connected = true;
}

void GameManager::disconnect()
{
    m_networker->disconnect();
    m_connected = false;

    m_server_root->remove_children();
    unregister_object(server_root_id);
}

bool GameManager::is_connected()
{
    return m_connected;
}

void GameManager::start()
{
    // Запускаем все системы
    for(std::shared_ptr<ISystem> system : m_systems)
    {
        system->start(this);
    }
    
    // Подключаемся к серверу
    // connect("localhost:8001");

    // Запускаем игровой цикл
    m_running = true;

    // Задаём время запуска
    m_start_time = std::chrono::steady_clock::now();

    game_loop();
}

void GameManager::game_loop()
{
    while(m_running)
    {
        // Объявляем буфер для принимаемый пакетов
        PackedData snapshot;

        // Если выполнено подключение к серверу
        if(m_connected)
        {
            // Получаем пакет с сервера
            snapshot = m_networker->receive_snapshot();
            
            // Распаковываем пакет
            unpack(snapshot);
        }

        // Запускаем итерацию каждой системы
        for(std::shared_ptr<ISystem> system : m_systems)
        {
            // Каждый раз проверяем, должен ли быть запущен цикл.
            // Это необходимо, потому что одна из систем может
            // вызвать GameManager::stop().
            if(m_running)
            {
                system->tick();
            } else {
                return;
            }
        }

        // Проверяем, пришел ли пакет с сервера
        if(m_connected && (snapshot.size() > 0))
        {
            // Создаём и отправляем ответ серверу
            m_networker->send_response(create_response());
        }

        // Ждём перед следующей итерацией
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void GameManager::unpack(PackedData data)
{
    // Проверяем, пуст ли пакет. Если да, то значит, что
    // ответ ещё не пришёл и распаковывать его не нужно.
    if(data.size() == 0)
    {
        return;
    }

    // Распаковываем количество объектов
    IntField object_count;
    object_count.unpack(data.take());

    // Удаляем дерево объектов сервера
    m_server_root->remove_children();
    unregister_object(server_root_id);

    // Распаковываем объекты
    std::vector< std::shared_ptr<Object> > unpacked_objects;
    for(int i = 0; i < object_count; i++)
    {
        auto object = std::make_shared<Object>(data.take(), this);
        m_id_to_object[object->get_id()] = object;

        unpacked_objects.push_back(object);
        m_objects.push_back(object);
    }

    // Обновляем указатели на родительские и дочерние объекты
    // у распакованных объектов
    for(std::shared_ptr<Object> object : unpacked_objects)
    {
        if(object->get_id() == server_root_id)
        {
            object->set_parent(nullptr);
            m_server_root = object;
        } else {
            auto parent = m_id_to_object[object->get_parent_id()];
            object->set_parent(parent);
            parent->register_child(object);
        }
    }

    // Очищаем данные пакета
    data.clear();
}

PackedData GameManager::create_response()
{
    // Создаём новый пакет
    PackedData data;
    data.set_keep_data(true);

    // Добавляем данные об изменении состояния контроллера
    data += m_controller->fetch_changes();

    return data;
}

std::shared_ptr<IComponent> GameManager::create_component(std::string name)
{
    return m_component_manager->create(name);
}

std::string GameManager::get_component_type_name(std::string component_name)
{
    return m_component_manager->get_type_name(component_name);
}

std::shared_ptr<Controller> GameManager::get_controller()
{
    return m_controller;
}

std::shared_ptr<Keyboard> GameManager::get_keyboard()
{
    return m_keyboard;
}

void GameManager::stop()
{
    debug("[GameManager::stop()] Stopping");

    m_running = false;

    // Останавливаем все системы
    for(std::shared_ptr<ISystem> system : m_systems)
    {
        system->stop();
    }

    if(m_connected)
    {
        disconnect();
    }
}

long long GameManager::get_client_time()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start_time).count();
}

}
