//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Server 4                               |  //
//  | Файл: GameManager.hpp                                  |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 07.04.2023                              |  //
//  | Дата изменения: 24.02.2024                             |  //
//  | Описание: GameManager - сердце игры.                   |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  |   Менеджер игры (GameManager) управляет добавлением,   |  //
//  | нахождением объектов, работой систем и работой с       |  //
//  | сетевым модулем. В его работе есть три основные фазы:  |  //
//  | 1) Настройка. В эту фазу никакие методы компонентов и  |  //
//  | систем не выполняются.                                 |  //
//  | 2) Запуск. Когда все системы добавлены, выполняются    |  //
//  | функции запуска всех систем.                           |  //
//  | 3) Цикл. Основная фаза игры, в которую системы         |  //
//  | работают, и клиенты могут подключаться.                |  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

#ifndef GameManager_hpp
#define GameManager_hpp

#include <map>
#include <unordered_map>
#include <memory>

#include "Object.hpp"
#include "IComponent.hpp"
#include "ISystem.hpp"
#include "INetworker.hpp"
#include "ComponentManager.hpp"
#include "Controller.hpp"

namespace engine
{

class GameManager
{
public:
    GameManager();
    
    /// Возвращает указатель на объект с данным уникальным номером.
    ///
    /// - Parameter id: Уникальный номер искомого объекта
    ///
    std::shared_ptr<Object> get_object(long long id);

    /// @brief Находит дочерний объект КОРНЕВОГО объекта
    /// @param name Имя искомого объекта
    /// @return Указатель на искомый объект
    std::shared_ptr<Object> get_object(std::string name);
    
    /// Возвращает указатель на корневой объект.
    std::shared_ptr<Object> get_root();
    
    /// Добавляет дочерний объект корневому объекту.
    std::shared_ptr<Object> add_object();
    
    /// Добавляет дочерний объект с данным именем корневому объекту.
    ///
    /// - Parameter name: Имя нового объекта
    ///
    std::shared_ptr<Object> add_object(std::string name);
    
    /// Добавляет дочерний объект объекту с данным номером.
    ///
    /// - Parameter parent_id: Уникальный номер родительского объекта
    ///
    std::shared_ptr<Object> add_object(long long parent_id);
    
    /// Добавляет дочерний объект с данным именем объекту с данным номером.
    ///
    /// - Parameter parent_id: Уникальный номер родительского объекта
    /// - Parameter name: Имя нового объекта
    ///
    std::shared_ptr<Object> add_object(long long parent_id, std::string name);

    /// @brief Удаляет объект и все его дочерние объекты
    /// @param object_id Уникальный номер удаляемого объекта
    ///
    void remove_object(long long object_id);
    
    /// @returns Массив всех объектов.
    std::vector<std::shared_ptr<Object>> get_objects();
    
    /// Добавляет систему.
    ///
    /// Работает только до начала игры.
    ///
    /// - Parameter system: Указатель на добавляемую систему
    ///
    void add_system(std::shared_ptr<ISystem> system);
    
    /// Запускает системы и входит в игровой цикл.
    void start();

    /// @brief Создаёт экземпляр компонента данного типа
    /// @param name Имя типа компонента
    /// @return Указатель на экземпляр компонента
    std::shared_ptr<IComponent> create_component(std::string name);

    /// @param client_id Номер клиента
    /// @return Указатель на его контроллер
    std::shared_ptr<Controller> get_controller(long long client_id);

    /// @return Список подключенных клиентов
    std::vector<Client> get_clients();
    
private:
    /// @brief Словарь вида ```номер объекта``` - ```объект```
    std::map<long long, std::shared_ptr<Object>> m_id_to_object;
    
    /// @brief Массив всех объектов
    std::vector<std::shared_ptr<Object>> m_objects;
    
    /// @brief Указатель на корневой объект
    std::shared_ptr<Object> m_root;
    
    /// @brief Максимальный использованный номер объекта
    long long m_max_id;
    
    /// @brief Запущена ли игра
    bool m_running;
    
    /// @brief Массив всех систем
    std::vector<std::shared_ptr<ISystem>> m_systems;
    
    /// @brief Указатель на используемый сетевой модуль
    std::shared_ptr<INetworker> m_networker;

    std::shared_ptr<ComponentManager> m_component_manager;

    /// @brief Словарь вида номер клиента - указатель на его контроллер.
    std::map<long long, std::shared_ptr<Controller>> m_controllers;
    
    /// Функция главного игрового цикла.
    void game_loop();

    /// @brief Упаковывает текущее состояние игрового мира
    /// @return Пакет состояния игрового мира
    PackedData pack();

    /// @brief Читает и обрабатывает ответы клиентов
    void read_responses();

    /// @brief Удаляет объект из внутреннего индекса
    /// @param object_id Уникальный номер объекта
    /// @note Используется при удалении объектов
    ///
    void unregister_object(long long object_id);

    // Класс Object должен иметь доступ к методу unregister_object
    friend Object;
};

}

#endif /* GameManager_hpp */
