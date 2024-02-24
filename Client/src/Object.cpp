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

Object::Object(long long id, long long parent_id, GameManager* game_manager)
{
    m_id = id;
    m_parent_id = parent_id;
    m_game_manager = game_manager;
}

Object::Object(PackedData data)
{
    m_id.unpack(data.take());
    m_name = data.take().data();
    m_parent_id.unpack(data.take());
    
    IntField components_size;
    components_size.unpack(data.take());
    m_components.clear();
    for(int i = 0; i < components_size; i++)
    {
        char *component_name = data.take().data();
        std::shared_ptr<IComponent> component = m_game_manager->create_component(component_name);
        PackedData component_data = data.take();
        component->unpack(component_data);
    }    
}

std::shared_ptr<IComponent> Object::get_component(std::string name)
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

std::shared_ptr<Object> Object::register_child(std::shared_ptr<Object> child)
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

void Object::clear_child_index()
{
    m_name_to_child.clear();
    m_id_to_child.clear();
    m_children.clear();
}

void Object::set_name(std::string name)
{
    m_name = name;
    auto parent = get_parent();
    if(parent)
    {
        parent->set_child_name(m_id, name);
    }
}

void Object::clear_name()
{
    auto parent = get_parent();
    if(parent)
    {
        parent->clear_child_name(m_name);
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
    return m_game_manager->get_object(m_parent_id);
}

PackedData Object::pack()
{
    PackedData data;
    data += m_id.pack();
    data += m_name;
    if(m_parent_id == -1)
    {
        data += IntField(-1).pack();
    } else {
        data += m_parent_id.pack();
    }
    
    data += IntField(m_components.size()).pack();
    for(auto elem : m_components)
    {
        data += elem.first;
        data += elem.second->pack();
    }
    
    return data;
}

void Object::unpack(PackedData data)
{
    // Распаковываем основные характеристики объекта 
    m_id.unpack(data.take());

    m_name = data.take().data();

    m_parent_id.unpack(data.take());

    // Удаляем все компоненты для замены на новые
    m_components.clear();

    // Распаковываем количество компонентов
    IntField component_count;
    component_count.unpack(data.take());

    // Распаковываем все компоненты
    for(int i = 0; i < component_count; i++)
    {
        // Распаковываем имя типа компонента
        std::string component_name = data.take().data();

        // Создаём компонент указанного типа
        std::shared_ptr<IComponent> component = m_game_manager->create_component(component_name);
        
        // Проверяем, удалось ли создать компонент
        if(component != nullptr)
        {
            // Если да, то распаковываем его данные
            component->unpack(data.take());
        } else {
            // Иначе просто пропускаем участок
            data.take();
        }
        
        // Добавляем компонент во внутренний индекс компонентов объекта
        std::string type_name = m_game_manager->get_component_type_name(component_name);
        m_components[type_name] = component;
    }
}

void Object::apply_changes(PackedData data)
{
    fixme("[Object::apply_changes] stub");
    unpack(data);
}

IntField Object::get_parent_id()
{
    return m_parent_id;
}

void Object::set_parent(std::shared_ptr<Object> parent)
{
    this->parent = parent;
}

}
