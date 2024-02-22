//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Server 4                               |  //
//  | Файл: Object.hpp                                       |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 31.03.2023                              |  //
//  | Дата изменения: 31.03.2023                             |  //
//  | Описание: Объявление Object - класса всех объектов.    |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  |   В игровом мире каждый объект задаётся данным классом.|  //
//  | Он хранит указатели на все свои компоненты и           |  //
//  | родительский объект.                                   |  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

#ifndef Object_hpp
#define Object_hpp

#include <memory>
#include <unordered_map>
#include <map>
#include <vector>

#include "IComponent.hpp"
#include "PackedData.hpp"
#include "Fields.hpp"
#include "utils/debug.hpp"

namespace engine
{

class GameManager;

class Object
{
public:
    Object(long long id, std::shared_ptr<Object> parent, GameManager* game_manager);
    
    template<typename T>
    /// @brief Возвращает указатель на компонент данного типа. Если такового нет
    /// в объекте, возвращает ```nullptr```.
    std::shared_ptr<T> get_component()
    {
        return std::dynamic_pointer_cast<T>(get_component(typeid(T).name()));
    }
    
    template<typename T>
    /// @brief Добавляет в объект компонент данного типа и возвращает указатель на него.
    /// Если такой уже существует, возвращает ```nullptr```.
    std::shared_ptr<T> add_component()
    {
        if(m_components.find(typeid(T).name()) != m_components.end())
        {
            error(std::string("[Object::add_component()] Attempting to add component ") + std::string(typeid(T).name()) + std::string(" to object ") + std::to_string(get_id()) + std::string(" but it already exists."));
            return nullptr;
        }
        
        m_components[typeid(T).name()] = std::make_shared<T>();
        return std::dynamic_pointer_cast<T>(m_components[typeid(T).name()]);
    }
    
    /// @brief Возвращает указатель на дочерний объект с данным именем. Если
    /// такового не существует, возвращает `nullptr`.
    ///
    /// @param name Имя искомого объекта
    ///
    std::shared_ptr<Object> get_child(std::string name);
    
    /// @brief Возвращает массив всех дочерних объектов.
    std::vector<std::shared_ptr<Object>> get_children();
    
    /// @brief Добавляет дочерний объект.
    ///
    /// @returns  Указатель на новый объект
    ///
    std::shared_ptr<Object> add_child();
    
    /// @brief Изменяет имя данного объекта.
    ///
    /// @param name Новое имя
    ///
    void set_name(std::string name);
    
    /// @brief Очищает имя объекта.
    ///
    /// После вызова имя объекта становится пустой строкой и удаляется из словаря
    /// имён родительского объекта.
    ///
    void clear_name();
    
    /// @brief Возвращает имя данного объекта.
    std::string get_name();
    
    /// @brief Возвращает уникальный номер данного объекта.
    IntField get_id();
    
    /// @brief Возвращает указатель на родительский объект данного объекта. Если
    /// данный объект корневой, то будет возвращён `nullptr`.
    std::shared_ptr<Object> get_parent();
    
    /// @brief упакованные данные объекта и его компонентов.
    PackedData pack();
    
    /// @brief изменения объекта и компонентов с прошлового вызова данного метода.
    PackedData fetch_changes();

    /// @brief Распаковывает данные
    /// @param data Упакованные данные
    void unpack(PackedData data);

    /// @brief Применяет изменения
    /// @param data Упакованные данные
    void apply_changes(PackedData data);
    
private:
    /// Словарь вида *имя типа компонента-указатель на него*
    std::unordered_map<std::string, std::shared_ptr<IComponent>> m_components;
    
    /// Словарь вида *имя дочернего объекта-указатель на него*
    std::unordered_map<std::string, std::shared_ptr<Object>> m_name_to_child;
    
    /// Словарь вида *уникальный номер дочернего объекта-указатель на него*
    std::map<long long, std::shared_ptr<Object>> m_id_to_child;
    
    /// Массив всех дочерних объектов
    std::vector<std::shared_ptr<Object>> m_children;
    
    /// Указатель на родительский объект. Если данный объект -- корневой, то принимает
    /// значение `nullptr`.
    std::shared_ptr<Object> m_parent;
    
    /// Имя данного объекта
    std::string m_name;
    
    /// Уникальный номер данного объекта. **НЕ ДОЛЖЕН МЕНЯТЬСЯ**
    IntField m_id;
    
    /// Указатель на игровой менеджер
    GameManager* m_game_manager;
    
    /// Возвращает указатель на компонент с данным именем.
    ///
    /// Если компонента с искомым именем нет в объекте, возвращает `nullptr`.
    ///
    /// - Parameter name: Имя типа компонента
    ///
    std::shared_ptr<IComponent> get_component(const char* name);
    
    /// Меняет имя дочернего объекта во внутреннем словаре имён.
    ///
    /// Вызывается только из метода `set_name()` дочернего объекта.
    /// Не меняет параметры самого дочернего объекта.
    ///
    /// - Parameters:
    ///   - id: Уникальный номер изменяемого объекта.
    ///   - name: Новое имя
    ///
    void set_child_name(long long id, std::string name);
    
    /// Удаляет дочерний объект из внутреннего словаря имён.
    ///
    /// Вызывается только из метода `clear_name()` дочернего объекта. **УДАЛЯЕМЫЙ ОБЪЕКТ
    /// ДОЛЖЕН ИМЕТЬ ИМЯ**.
    /// Не меняет параметры самого дочернего объекта.
    ///
    ///  - Parameters:
    ///    - name: Имя удаляемого объекта
    ///
    void clear_child_name(std::string name);

    /// @brief Добавляет дочерний объект во внутренний индекс
    ///
    /// Вызывается только из метода `GameObject::add_object()`. Добавляет
    /// дочерний объект во внутренний индекс.
    ///
    /// @param child Указатель на объект, который нужно добавить.
    ///
    void register_child(std::shared_ptr<Object> child);

    void deregister_child(long long child_id);
    
    // GameManager должен иметь доступ к методам register_child и
    // clear_child_name
    friend GameManager;

};

}

#endif /* Object_hpp */
