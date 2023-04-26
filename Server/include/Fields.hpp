//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Server 4                               |  //
//  | Файл: Fields.hpp                                       |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 03.04.2023                              |  //
//  | Дата изменения: 03.04.2023                             |  //
//  | Описание: Объявление часто встречающихся полей.        |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  |   Тип IntField копирует поведение long long, но имеет  |  //
//  | методы pack() и fetch_changes(). Является реализацией  |  //
//  | интерфейса IField.                                     |  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

#ifndef IntField_hpp
#define IntField_hpp

#include "IField.hpp"

namespace engine
{

class IntField : public IField
{
public:
    IntField();
    IntField(long long number);
    PackedData pack() override;
    PackedData fetch_changes() override;
    
    IntField operator += (IntField b);
    IntField operator -= (IntField b);
    
    operator long long ();
    
private:
    long long m_number;
    long long m_prev;
};

class Vec2Field : public IField
{
public:
    Vec2Field();
    Vec2Field(IntField x, IntField y);
    PackedData pack() override;
    PackedData fetch_changes() override;
    
    void operator += (Vec2Field b);
    
    IntField x, y;
};

}

#endif /* IntField_hpp */
