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

#include <string>

#include "IField.hpp"

namespace engine
{

class IntField : public IField
{
public:
    IntField();
    IntField(int number);
    PackedData pack() override;
    PackedData fetch_changes() override;
    void unpack(PackedData data) override;
    void apply_changes(PackedData data) override;
    
    IntField operator += (IntField b);
    IntField operator -= (IntField b);
    
    operator int ();
    
private:
    int m_number;
    int m_prev;
};

class Vec2Field : public IField
{
public:
    Vec2Field();
    Vec2Field(IntField x, IntField y);
    PackedData pack() override;
    PackedData fetch_changes() override;
    void unpack(PackedData data) override;
    void apply_changes(PackedData data) override;
    
    void operator += (Vec2Field b);
    void operator -= (Vec2Field b);
    
    IntField x, y;
};

class StringField : public IField
{
public:
    StringField();
    StringField(std::string string);

    PackedData pack() override;
    PackedData fetch_changes() override;
    void unpack(PackedData data) override;
    void apply_changes(PackedData data) override;

    operator std::string();
    
    std::string &s();

private:
    std::string m_string;
};

}

#endif /* IntField_hpp */
