//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Server 4                               |  //
//  | Файл: ComponentManager.hpp                             |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 27.04.2023                              |  //
//  | Дата изменения: 11.05.2023                             |  //
//  | Описание: Модуль управления компонентами               |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  |   Данный модуль отвечает за регистрацию и последующее  |  //
//  | создание экземпляров компонентов. Используется при     |  //
//  | распаковке данных и создании объектов на их основе.    |  //
//  |                                                        |  //
//  |--------------------------------------------------------|  //
//  | ПРИМЕЧАНИЯ                                             |  //
//  | 1. Пока используется только на стороне клиента.        |  //
//  | Серверная релализация добавлена для будущего развития. |  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

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
    template<typename T>
    void register_component(std::string name)
    {
        if(m_factories.find(name) != m_factories.end())
        {
            warning(std::string("[ComponentManager.register_component] Component type with name '")
            + name
            + "' has already been registered");
        }
        m_factories[name] = &ComponentManager::factory<T>;
    }

    std::shared_ptr<IComponent> create(std::string name)
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
    // std::unordered_map<const char*, std::function<std::shared_ptr<IComponent>(ComponentManager*)>> m_factories;
    std::unordered_map<std::string, std::shared_ptr<engine::IComponent> (*)()> m_factories;

    template<typename T>
    static std::shared_ptr<IComponent> factory()
    {
        return std::static_pointer_cast<IComponent>(std::make_shared<T>());
    }
};

}

#endif