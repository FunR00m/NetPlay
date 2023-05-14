//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Client 4                               |  //
//  | Файл: Controller.hpp                                   |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 12.05.2023                              |  //
//  | Дата изменения: 12.05.2023                             |  //
//  | Описание: Модуль контроля сосотояния клавиатуры        |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  |   Для удобного взаимодействия с устройствами управления|  //
//  | можно использовать данный модуль. Он принимает         |  //
//  | информацию об изменениях состояний клавиш от модуля    |  //
//  | отрисовки и может передать её сетевому модулю.         |  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <map>
#include <vector>

#include "PackedData.hpp"

namespace engine
{

/// @brief Модуль контроля состояния клавиатуры
class Controller
{
public:
    /// @brief Перечисление кодов клавиш
    /// @note Коды клавиш такие же, как и в SDL2/SDL_keycode.h
    enum Key
    {
        UNKNOWN = 0,
        RETURN = '\r',
        ESCAPE = '\x1B',  
        BACKSPACE = '\b',
        TAB = '\t',       
        SPACE = ' ',
        EXCLAIM = '!',
        QUOTEDBL = '"',
        HASH = '#',
        PERCENT = '%',
        DOLLAR = '$',
        AMPERSAND = '&',
        QUOTE = '\'',
        LEFTPAREN = '(',
        RIGHTPAREN = ')',
        ASTERISK = '*',
        PLUS = '+',
        COMMA = ',',
        MINUS = '-',
        PERIOD = '.',
        SLASH = '/',
        N_0 = '0',
        N_1 = '1',
        N_2 = '2',
        N_3 = '3',
        N_4 = '4',
        N_5 = '5',
        N_6 = '6',
        N_7 = '7',
        N_8 = '8',
        N_9 = '9',
        COLON = ':',
        SEMICOLON = ';',
        LESS = '<',
        EQUALS = '=',
        GREATER = '>',
        QUESTION = '?',
        AT = '@',
        LEFTBRACKET = '[',
        BACKSLASH = '\\',
        RIGHTBRACKET = ']',
        CARET = '^',
        UNDERSCORE = '_',
        BACKQUOTE = '`',
        a = 'a',
        b = 'b',
        c = 'c',
        d = 'd',
        e = 'e',
        f = 'f',
        g = 'g',
        h = 'h',
        i = 'i',
        j = 'j',
        k = 'k',
        l = 'l',
        m = 'm',
        n = 'n',
        o = 'o',
        p = 'p',
        q = 'q',
        r = 'r',
        s = 's',
        t = 't',
        u = 'u',
        v = 'v',
        w = 'w',
        x = 'x',
        y = 'y',
        z = 'z',
    };

    Controller();

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

    /// @brief Упаковывает данные о состоянии всех клавиш
    /// @return Упакованные данные клавиатуры
    PackedData pack();

    /// @return Упакованные данные изменившихся с момента последнего
    /// вызова этой функции клавиш.
    PackedData fetch_changes();

private:
    /// @brief Словарь текущий состояний клавиш
    std::map<Key, bool> key_states;

    /// @brief Список клавиш, изменивших состояние с момента
    /// последнего вызова fetch_chages()
    std::vector<Key> changed_keys;
    
};

}


#endif