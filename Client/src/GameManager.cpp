//
//  GameManager.cpp
//  Client
//
//  Created by Fedor Fedor on 20.04.2023.
//

#include <memory>
#include <chrono>
#include <thread>

#include "GameManager.hpp"

namespace engine
{

GameManager::GameManager()
{
    m_root = std::make_shared<Object>(0, nullptr, this);
    m_id_to_object[0] = m_root;
    m_objects.push_back(m_root);
    
    m_max_id = 0;
    
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
    auto new_object = std::make_shared<Object>(m_max_id, m_id_to_object[parent_id], this);
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
    game_loop();
}

void GameManager::game_loop()
{
    while(m_running)
    {
        for(std::shared_ptr<ISystem> system : m_systems)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            system->tick();
        }
    }
}

}
