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
    m_root = std::make_shared<Object>(0, -1, this);
    m_id_to_object[0] = m_root;
    m_objects.push_back(m_root);
    
    m_max_id = 0;

    m_networker = std::make_shared<sys::InetNetworker>();
    m_component_manager = std::make_shared<ComponentManager>();
    
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
    m_id_to_object[parent_id]->add_child(new_object);
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
    for(std::shared_ptr<ISystem> system : m_systems)
    {
        system->start(this);
    }
    
    m_running = true;
    m_networker->connect("localhost:8001");
    game_loop();
}

void GameManager::game_loop()
{
    while(m_running)
    {
        unpack(m_networker->receive_snapshot());
        for(std::shared_ptr<ISystem> system : m_systems)
        {
            system->tick();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        m_networker->send_response(PackedData((void*)"1", 1));
    }
}

void GameManager::unpack(PackedData data)
{
    if(data.get_size() == 0)
    {
        return;
    }

    IntField object_count;
    object_count.unpack(data.take());

    m_objects.clear();
    for(int i = 0; i < object_count; i++)
    {
        auto object = add_object();
        object->unpack(data.take());
    }
}

std::shared_ptr<IComponent> GameManager::create_component(std::string name)
{
    return m_component_manager->create(name);
}

}
