#include <memory>
#include <functional>
#include <unordered_map>

#include "IComponent.hpp"
#include "utils/debug.hpp"

#ifndef ComponentManager_hpp
#define ComponentManager_hpp

namespace engine
{

class ComponentManager
{
public:
    /// @brief Регистрирует компонент и задаёт его имя.
    /// @tparam T Тип регистрируемого компонента
    /// @param name Имя компонента
    template<typename T>
    void register_component(std::string name)
    {
        if(name == "UNDEF_COMPONENT")
        {
            error("[ComponentManager::register_component] UNDEF_COMPONENT is a reserved component name. It can not be used.");
            return;
        }
        if(m_factories.find(name) != m_factories.end())
        {
            warning(std::string("[ComponentManager::register_component] Component type with name '")
            + name
            + "' has already been registered");
        }
        m_factories[name] = &ComponentManager::factory<T>;
        m_name_to_type[name] = typeid(T).name();
    }

    /// @brief Создаёт компонент по зарегистрировнному имени.
    /// @param name Имя компонента
    /// @return Указатель на созданный компонент
    std::shared_ptr<IComponent> create(std::string name);

    /// @brief Возвращает имя типа компонента.
    /// @param component_name Зарегистрированное название компонента
    /// @return Имя типа компонента
    std::string get_type_name(std::string component_name); 

private:
    // std::unordered_map<const char*, std::function<std::shared_ptr<IComponent>(ComponentManager*)>> m_factories;
    
    /// @brief Словарь вида имя компонента -- указатель на создовающую его функцию
    std::unordered_map<std::string, std::shared_ptr<engine::IComponent> (*)()> m_factories;

    /// @brief Словарь вида имя компонента -- название его типа
    std::unordered_map<std::string, std::string> m_name_to_type;

    template<typename T>
    static std::shared_ptr<IComponent> factory()
    {
        return std::static_pointer_cast<IComponent>(std::make_shared<T>());
    }
};

}

#endif