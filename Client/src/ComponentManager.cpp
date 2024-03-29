#include "ComponentManager.hpp"
#include "utils/debug.hpp"

namespace engine
{


std::shared_ptr<IComponent> ComponentManager::create(std::string name)
{
    if(m_factories.find(name) == m_factories.end())
    {
        if(!m_already_warned.contains(name))
        {
            error(std::string("[ComponentManager::create] Component type with name '")
            + name
            + "' has not been registered");
            m_already_warned.insert(name);
        }
        return nullptr;
    }
    return m_factories[name]();
}

std::string ComponentManager::get_type_name(std::string component_name)
{
    if(m_name_to_type.find(component_name) == m_name_to_type.end())
    {
        if(!m_already_warned.contains(component_name))
        {
            warning("[ComponentManager::get_type_name] Unable to find type name for component "
            + component_name + ".");
            m_already_warned.insert(component_name);
        }
        return "UNDEF_COMPONENT";
    }

    return m_name_to_type[component_name];
}

}
