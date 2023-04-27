#include <memory>
#include <functional>
#include <unordered_map>

#include "IComponent.hpp"

namespace engine
{

class ComponentManager
{
public:
    template<typename T>
    void register_component()
    {
        const char *name = typeid(T).name();
        if(m_factories.find(name) != m_factories.end())
        {
            warning(std::string("[ComponentManager.register_component] Component type with name '")
            + name
            + "' has already been registered");
        }
        m_factories[name] = factory<T>();
    }

    std::shared_ptr<IComponent> create(const char *name)
    {
        if(m_factories.find(name) == m_factories.end())
        {
            error(std::string("[ComponentManager.register_component] Component type with name '")
            + name
            + "' has not been registered");
            return nullptr;
        }
        return m_factories[name]();
    }

private:
    std::unordered_map<const char*, std::function<std::shared_ptr<IComponent>()>> m_factories;

    template<typename T>
    std::shared_ptr<IComponent> factory()
    {
        return std::make_shared<T>();
    }
};

}