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
    // Создаём корневой объект
    m_root = std::make_shared<Object>(0, -1, this);
    m_id_to_object[0] = m_root;
    m_objects.push_back(m_root);
    
    // Устанавливаем максимальный использованный уникальный номер
    m_max_id = 0;

    // Создаём экземпляр сетевого модуля
    m_networker = std::make_unique<sys::InetNetworker>();

    // Создаём экземпляр менеджера компонентов
    m_component_manager = std::make_shared<ComponentManager>();

    // Создаём экземпляр контроллера
    m_controller = std::make_shared<Controller>();
    
    m_running = false;
}

std::shared_ptr<Object> GameManager::get_root()
{
    return m_root;
}

std::shared_ptr<Object> GameManager::add_object(long long parent_id)
{
    if(m_id_to_object.find(parent_id) == m_id_to_object.end())
    {
        error("[GameManager::add_object(" + std::to_string(parent_id) + ")] Object with given parent ID does not exist.");
        return nullptr;
    }
    m_max_id += 1;
    auto new_object = std::make_shared<Object>(m_max_id, parent_id, this);
    m_id_to_object[m_max_id] = new_object;
    m_id_to_object[parent_id]->register_child(new_object);
    m_objects.push_back(new_object);
    
    return new_object;
}

std::shared_ptr<Object> GameManager::add_object()
{
    return add_object(0);
}

std::shared_ptr<Object> GameManager::add_object(long long parent_id, std::string name)
{
    auto new_object = add_object(parent_id);
    new_object->set_name(name);
    
    return new_object;
}

std::shared_ptr<Object> GameManager::add_object(std::string name)
{
    return add_object(0, name);
}

std::shared_ptr<Object> GameManager::get_object(long long id)
{
    if(m_id_to_object.find(id) == m_id_to_object.end())
    {
        warning("[GameManager::get_object] Object with given ID was not found.");
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

void GameManager::start()
{
    // Запускаем все системы
    for(std::shared_ptr<ISystem> system : m_systems)
    {
        system->start(this);
    }
    
    // Подключаемся к серверу
    m_networker->connect("localhost:8001");

    // Запускаем игровой цикл
    m_running = true;
    game_loop();
}

void GameManager::game_loop()
{
    while(m_running)
    {
        // Получаем пакет с сервера
        PackedData snapshot = m_networker->receive_snapshot();
        
        // Распаковываем пакет
        unpack(snapshot);

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
        if(snapshot.size() > 0)
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

    // Полностью заменяем объекты
    m_objects.clear();
    for(int i = 0; i < object_count; i++)
    {
        // Создаём новый объект из данных пакета
        auto object = add_object();
        object->unpack(data.take());
    }

    // Обновляем индекс объектов
    m_id_to_object.clear();
    for(std::shared_ptr<Object> object : m_objects)
    {
        // Очищает индекс дочених объектов для следующей стадии
        object->clear_child_index();

        m_id_to_object[object->get_id()] = object;
    }

    // Обновляем указатели на родительские объекты
    for(std::shared_ptr<Object> object : m_objects)
    {
        if(object->get_parent_id() == -1)
        {
            object->set_parent(nullptr);
        } else {
            object->set_parent(m_id_to_object[object->get_parent_id()]);
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

void GameManager::stop()
{
    debug("[GameManager::stop()] Stopping");

    m_running = false;

    // Останавливаем все системы
    for(std::shared_ptr<ISystem> system : m_systems)
    {
        system->stop();
    }

    m_networker->disconnect();
}

}
