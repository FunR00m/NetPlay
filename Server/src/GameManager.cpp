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
    m_root = std::make_shared<Object>(0, nullptr, this);
    m_id_to_object[0] = m_root;
    m_objects.push_back(m_root);
    
    // Устанавливаем максимальный использованный уникальный номер
    m_max_id = 0;

    // Создаём экземпляр сетевого модуля
    m_networker = std::make_shared<sys::InetNetworker>();
    
    m_running = false;
}

std::shared_ptr<Object> GameManager::get_root()
{
    return m_root;
}

std::shared_ptr<Object> GameManager::add_object(long long parent_id)
{
    // Проверяем, существует ли запрошенный родительский объект
    if(m_id_to_object.find(parent_id) == m_id_to_object.end())
    {
        error("[GameManager::add_object(" + std::to_string(parent_id) + ")] Object with given parent ID does not exist.");
        return nullptr;
    }

    // Находим родительский объект
    std::shared_ptr<Object> parent = m_id_to_object[parent_id];

    // Изменияем максимальный уникальный номер
    m_max_id += 1;

    // Создаём экземпляр объекта
    auto new_object = std::make_shared<Object>(m_max_id, parent, this);

    // Добавляем объект в индекс
    m_id_to_object[m_max_id] = new_object;
    m_objects.push_back(new_object);

    // Добавляем объект в индекс родительского объекта
    parent->add_child(new_object);
    
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
    
    m_running = true;
    m_networker->start_listening();
    game_loop();
}

void GameManager::game_loop()
{
    while(m_running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        read_responses();
        for(std::shared_ptr<ISystem> system : m_systems)
        {
            system->tick();
        }

        m_networker->send_snapshot(pack());
    }
}

PackedData GameManager::pack()
{
    PackedData data;

    // Добавляем количество последующих объектов
    data += IntField(m_objects.size()).pack();

    // Добавляем упакованные данные всех объектов
    for(auto object : m_objects)
    {
        data += object->pack();
    }

    return data;
}

std::shared_ptr<IComponent> GameManager::create_component(std::string name)
{
    return m_component_manager->create(name);
}

std::vector<Client> GameManager::get_clients()
{
    return m_networker->get_clients();
}

std::shared_ptr<Controller> GameManager::get_controller(long long client_id)
{
    if(m_controllers.find(client_id) == m_controllers.end())
    {
        error(std::string("[GameManager::get_controller] Unable to find controller for client ")
        + std::to_string(client_id) + ".");
        return nullptr;
    }

    return m_controllers[client_id];
}

void GameManager::read_responses()
{
    std::vector<Client> clients = m_networker->get_clients();

    for(Client client : clients)
    {
        if(m_controllers.find(client.id) == m_controllers.end())
        {
            m_controllers[client.id] = std::make_shared<Controller>();
        }
        
        PackedData data = m_networker->get_response(client.id);
        if(data.size() > 0)
        {
            m_controllers[client.id]->apply_changes(data.take());
        }

        data.clear();
    }
}

}
