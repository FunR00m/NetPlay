//
//  Fields.cpp
//  Server
//
//  Created by Fedor Fedor on 03.04.2023.
//

#include "Fields.hpp"

namespace engine
{

IntField::IntField()
{
    m_number = 0;
    m_prev = -1;
}

IntField::IntField(long long number)
{
    m_number = number;
    m_prev = number - 1;
}
IntField::operator long long()
{
    return m_number;
}

PackedData IntField::pack()
{
    return PackedData((void*)m_number, sizeof(m_number));
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

void Vec2Field::operator += (Vec2Field b)
{
    x += b.x;
    y += b.y;
}

}
