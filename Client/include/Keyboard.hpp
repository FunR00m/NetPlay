//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Client 4                               |  //
//  | Файл: Keyboard.hpp                                     |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 26.02.2024                              |  //
//  | Дата изменения: 26.02.2024                             |  //
//  | Описание: Модуль контроля сосотояния клавиатуры        |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  |   Для удобного взаимодействия с устройствами управления|  //
//  | можно использовать данный модуль. Он принимает         |  //
//  | информацию об изменениях состояний клавиш от модуля    |  //
//  | отрисовки и может передать её контроллеру.             |  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <map>
#include <vector>

#include "PackedData.hpp"
#include "Controller.hpp"

namespace engine
{

/// @brief Модуль контроля состояния клавиатуры
class Keyboard
{
public:
    using Key = uint32_t;

    /// @brief Конструирует класс
    /// @param controller Указатель на контроллер, к котрому
    /// будет прикреплена клавиатура.
    Keyboard(std::shared_ptr<Controller> controller);

    /// @brief Устанавливает состояние клавиши
    /// @param key Изменяемая клавиша
    /// @param state Новое состояние.
    /// true - нажата,
    /// false - не нажата.
    void set_state(Key key, bool state);
    
    /// @brief Устанавливает состояние клавиши на "нажата"
    /// @param key Нажимаемая клавиша
    void press(Key key);

    /// @brief Устанавливает состояние клавиши на "не нажата"
    /// @param key Отпускаемая клавиша
    void release(Key key);

    /// @param key Номер клавиши
    /// @return Текущее состояние данной клавиши.
    /// true - нажата,
    /// false - не нажата.
    bool get_state(Key key);

    /// @brief Назначает элемент управления на клавишу
    /// @param key Клавиша
    /// @param control Элемент управления
    void assign_control(Key key, Controller::Control control);

    /// @brief Убирает элемент управления, назначенный на клавишу
    /// @param key 
    void unassign_key(Key key);

private:
    /// @brief Словарь текущих состояний клавиш
    std::map<Key, bool> m_key_states;

    /// @brief Словарь назначенных клавиш
    std::map<Key, Controller::Control> m_assigned_controls;
    
    /// @brief Указатель на используемый контроллер
    std::shared_ptr<Controller> m_controller;
    
};

}


#endif