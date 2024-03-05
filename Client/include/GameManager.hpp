//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Client 4                               |  //
//  | Файл: GameManager.hpp                                  |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 07.04.2023                              |  //
//  | Дата изменения: 07.04.2023                             |  //
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
//  | работают.                                              |  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <map>
#include <unordered_map>
#include <memory>
#include <chrono>

#include "Object.hpp"
#include "IComponent.hpp"
#include "ISystem.hpp"
#include "INetworker.hpp"
#include "ComponentManager.hpp"
#include "Controller.hpp"
#include "Keyboard.hpp"

namespace engine
{

const long long server_root_id = 0;
const long long client_root_id = 4096;

class GameManager
{
public:
    GameManager();
    
    /// Возвращает указатель на объект с данным уникальным номером.
    ///
    /// - Parameter id: Уникальный номер искомого объекта
    ///
    std::shared_ptr<Object> get_object(long long id);
    
    /// @returns Указатель на корневой объект сервера
    std::shared_ptr<Object> get_server_root();
    
    /// @returns Указатель на корневой объект клиента
    std::shared_ptr<Object> get_client_root();
    
    /// @returns Указатель на корневой объект клиента
    std::shared_ptr<Object> get_root();
    
    /// Добавляет дочерний объект корневому объекту сервера
    std::shared_ptr<Object> add_server_object();
    
    /// Добавляет дочерний объект корневому объекту клиента
    std::shared_ptr<Object> add_client_object();
    
    /// Добавляет дочерний объект с данным именем корневому объекту клиента.
    ///
    /// - Parameter name: Имя нового объекта
    ///
    std::shared_ptr<Object> add_client_object(std::string name);
    
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
    
    /// Возвращает массив всех объектов.
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

    /// @brief Создаёт экземпляр компонента
    /// @param name Зарегистрированное имя компонента
    /// @return Указатель на созданный экземпляр компонента
    std::shared_ptr<IComponent> create_component(std::string name);

    /// @brief Регистрирует компонент
    /// @tparam T Тип компонента
    /// @param name Имя компонента
    /// @note Имена компонента на обеих сторонах должны совпадать.
    template<typename T>
    void register_component(std::string name)
    {
        m_component_manager->register_component<T>(name);
    }

    /// @return Имя типа компонента, полученное с помощью typeid()
    /// @param component_name Зарегистрированное имя компонента
    std::string get_component_type_name(std::string component_name);

    /// @return Указатель на используемый контроллер
    std::shared_ptr<Controller> get_controller();

    /// @return Указатель на используемую клавиатуру
    std::shared_ptr<Keyboard> get_keyboard();

    /// @brief Останавливает все системы и модули
    void stop();

    /// @brief Подключает к серверу
    /// @param adress Адрес сервера
    void connect(std::string adress);

    /// @brief Отключает от сервера
    void disconnect();

    /// @return Подключен ли клиент к серверу
    bool is_connected();

    /// @return Количество миллисекунд, прошедших с запуска
    long long get_client_time();
    
private:
    /// Словарь вида *номер объекта-объект*
    std::map<long long, std::shared_ptr<Object>> m_id_to_object;
    
    /// Массив всех объектов
    std::vector<std::shared_ptr<Object>> m_objects;
    
    /// Указатель на корневой объект сервера
    std::shared_ptr<Object> m_server_root;
    
    /// Указатель на корневой объект клиента
    std::shared_ptr<Object> m_client_root;
    
    /// Максимальный занятый номер объекта на стороне клиента
    long long m_max_id;
    
    /// Запущена ли игра
    bool m_running;

    /// Подключен ли клиент к серверу
    bool m_connected;
    
    /// Массив всех систем
    std::vector<std::shared_ptr<ISystem>> m_systems;
    
    /// @brief Указатель на сетевой модуль
    std::unique_ptr<INetworker> m_networker;

    /// @brief Указатель на модуль управления компонентами
    std::shared_ptr<ComponentManager> m_component_manager;

    /// @brief Указатель на текущий контроллер
    std::shared_ptr<Controller> m_controller;

    /// @brief Указатель на клавиатуру
    std::shared_ptr<Keyboard> m_keyboard;

    std::chrono::steady_clock::time_point m_start_time;
    
    /// Функция главного игрового цикла.
    void game_loop();

    /// @brief Распаковывает снимок, принятый с сервера
    /// @param data Упакованные данные снимка
    void unpack(PackedData data);

    /// @brief Генерирует ответ для сервера
    /// @return Упакованные данные ответа
    PackedData create_response();

    /// @brief Удаляет объект из внутреннего индекса
    /// @param object_id Уникальный номер объекта
    /// @note Используется при удалении объектов
    ///
    void unregister_object(long long object_id);

    // Класс Object должен иметь доступ к методу unregister_object
    friend Object;
};

}

#endif
