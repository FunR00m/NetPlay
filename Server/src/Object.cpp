//
//  Object.cpp
//  Server
//
//  Created by Fedor Fedor on 02.04.2023.
//

#include "Object.hpp"
#include "GameManager.hpp"
#include "utils/debug.hpp"

namespace engine
{

Object::Object(long long id, std::shared_ptr<Object> parent, GameManager* game_manager)
{
    m_id = id;
    m_parent = parent;
    m_game_manager = game_manager;
}

std::shared_ptr<IComponent> Object::get_component(const char* name)
{
    if(m_components.find(name) == m_components.end())
    {
        //warning(std::string("[Object::get_component(\"") + std::string(name) + std::string("\")] Component not found. Object ID: " + std::to_string(m_id)));
        return nullptr;
    }
    
    return m_components[name];
}

std::vector<std::shared_ptr<Object>> Object::get_children()
{
    return m_children;
}

std::shared_ptr<Object> Object::get_child(std::string name)
{
    if(m_name_to_child.find(name) == m_name_to_child.end())
    {
        warning(std::string("[Object::get_child(\"") + std::string(name) + std::string("\")] Child not found. Object ID: " + std::to_string(m_id)));
        return nullptr;
    }
    
    return m_name_to_child[name];
}

std::shared_ptr<Object> Object::add_child()
{
    std::shared_ptr<Object> new_object = m_game_manager->add_object(m_id);
    m_children.push_back(new_object);
    m_id_to_child[new_object->get_id()] = new_object;
    return new_object;
}

std::shared_ptr<Object> Object::add_child(std::shared_ptr<Object> child)
{
    if(child->get_parent()->get_id() != m_id)
    {
        error("[Object::add_child(child)] Current ID and child's ID does not match! Current object ID: " + std::to_string(m_id));
        return nullptr;
    }
    m_children.push_back(child);
    m_id_to_child[child->get_id()] = child;
    if(!child->get_name().empty())
    {
        m_name_to_child[child->get_name()] = child;
    }
    
    return child;
}

void Object::set_name(std::string name)
{
    m_name = name;
    if(m_parent)
    {
        m_parent->set_child_name(m_id, name);
    }
}

void Object::clear_name()
{
    if(m_parent)
    {
        m_parent->clear_child_name(m_name);
    }
    m_name.clear();
}

void Object::set_child_name(long long id, std::string name)
{
    if(m_id_to_child.find(id) == m_id_to_child.end())
    {
        error(std::string("[Object::set_child_name(" + std::to_string(id) + ", \"") + std::string(name) + std::string("\")] Child with given id does not exist. Ignoring. Current object ID: " + std::to_string(m_id)));
        return;
    }
    
    std::shared_ptr<Object> child = m_id_to_child[id];
    
    if(m_name_to_child.find(name) != m_name_to_child.end())
    {
        warning(std::string("[Object::set_child_name(" + std::to_string(id) + ", \"") + std::string(name) + std::string("\")] Child with given name already exists. Clearing its name. Current object ID: " + std::to_string(m_id)));
        m_name_to_child[name]->clear_name();
    }
    
    m_name_to_child.erase(child->get_name());
    m_name_to_child[name] = child;
}

void Object::clear_child_name(std::string name)
{
    if(m_name_to_child.find(name) == m_name_to_child.end())
    {
        error(std::string("[Object::clear_child_name(\"" + name + std::string("\")] Child with given name does not exist. Ignoring. Current object ID: ") + std::to_string(m_id)));
        return;
    }
    m_name_to_child.erase(name);
}

std::string Object::get_name()
{
    return m_name;
}

IntField Object::get_id()
{
    return m_id;
}

std::shared_ptr<Object> Object::get_parent()
{
    return m_parent;
}

PackedData Object::pack()
{
    PackedData data;
    data += m_id.pack();
    data += m_name;
    data += m_parent->get_id().pack();
    
    data += IntField(m_components.size()).pack();
    for(auto elem : m_components)
    {
        data += elem.second->pack();
    }
    
    return data;
}

}
