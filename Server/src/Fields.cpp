//
//  Fields.cpp
//  Server
//
//  Created by Fedor Fedor on 03.04.2023.
//

#include "Fields.hpp"
#include "utils/debug.hpp"

namespace engine
{

IntField::IntField()
{
    m_number = 0;
    m_prev = -1;
}

IntField::IntField(int number)
{
    m_number = number;
    m_prev = number - 1;
}
IntField::operator int()
{
    return m_number;
}

PackedData IntField::pack()
{
    return PackedData((void*)&m_number, sizeof(m_number));
}

PackedData IntField::fetch_changes()
{
    if(m_prev == m_number)
    {
        return PackedData();
    } else {
        m_prev = m_number;
        return pack();
    }
}

void IntField::unpack(PackedData data)
{
    if(data.get_size() != sizeof(m_number))
    {
        error("[IntField::unpack] Unable to unpack the data.");
    }
    m_number = *(int*) data.get_data().data();
}

void IntField::apply_changes(PackedData data)
{
    if(data.get_size() == 0)
    {
        return;
    } else if(data.get_size() == sizeof(m_number))
    {
        unpack(data);
    } else {
        error("[IntField::apply_changes] Unable to apply changes.");
    }
}

IntField IntField::operator += (IntField b)
{
    m_number += b;
    return m_number;
}

IntField IntField::operator -= (IntField b)
{
    m_number -= b;
    return m_number;
}


Vec2Field::Vec2Field()
{
    x = 0;
    y = 0;
}

Vec2Field::Vec2Field(IntField x, IntField y)
{
    this->x = x;
    this->y = y;
}

PackedData Vec2Field::pack()
{
    PackedData data;
    data += x.pack();
    data += y.pack();
    return data;
}

PackedData Vec2Field::fetch_changes()
{
    PackedData data;
    data += x.fetch_changes();
    data += y.fetch_changes();
    return data;
}

void Vec2Field::unpack(PackedData data)
{
    x.unpack(data.take());
    y.unpack(data.take());
}

void Vec2Field::apply_changes(PackedData data)
{
    x.apply_changes(data.take());
    y.apply_changes(data.take());
}

void Vec2Field::operator += (Vec2Field b)
{
    x += b.x;
    y += b.y;
}


StringField::StringField()
{
    m_string.clear();
}

StringField::StringField(std::string string)
{
    m_string = string;
}

StringField::operator std::__1::string()
{
    return m_string;
}

PackedData StringField::pack()
{
    PackedData data;
    data += m_string;
    return data;
}

PackedData StringField::fetch_changes()
{
    fixme("[StringField::fetch_changes] stub");
    return pack();
}

void StringField::unpack(PackedData data)
{
    std::vector<char> string_data = data.take().get_data();
    string_data.push_back(0);                                   // Заключающий ноль
    m_string = string_data.data();                              // Создание строки из нуль-терминированной строки
}

void StringField::apply_changes(PackedData data)
{
    fixme("[StringField::apply_changes] stub");
    return unpack(data);
}

std::string &StringField::s()
{
    return m_string;
}

}
