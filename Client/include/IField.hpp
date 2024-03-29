//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Server 4                               |  //
//  | Файл: IField.hpp                                       |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 02.04.2023                              |  //
//  | Дата изменения: 02.04.2023                             |  //
//  | Описание: Интерфейс полей компонентов.                 |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  |   Для передачи данных компонентов клиенту, они должны  |  //
//  | упакованы в последовательность байтов. Поля могут      |  //
//  | хранить разную информацию и легко преобразовывать её в |  //
//  | объект PackedData.                                     |  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

#ifndef IFIELD_HPP
#define IFIELD_HPP

#include "PackedData.hpp"

namespace engine
{

class IField
{
public:
    virtual PackedData pack() = 0;
    virtual PackedData fetch_changes() = 0;
    virtual void unpack(PackedData data) = 0;
    virtual void apply_changes(PackedData data) = 0;
};

}

#endif /* IField_h */
