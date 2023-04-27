//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Server 4                               |  //
//  | Файл: PackedData.cpp                                   |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 02.04.2023                              |  //
//  | Дата изменения: 02.04.2023                             |  //
//  | Описание: Последовательность байт, готовая к передаче. |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  | См. PackedData.hpp                                     |  //
//  ==========================================================  //
//                                                              //

#include "PackedData.hpp"
#include "utils/debug.hpp"

namespace engine
{

PackedData::PackedData()
{
    m_size = 0;
}

PackedData::PackedData(std::vector<char> &data)
{
    m_data = data;
    m_size = data.size();
}

PackedData::PackedData(void* object, DataSize size)
{
    m_size = size;
    
    for(int i = 0; i < size; i++)
    {
        m_data.push_back(*(((char*) object) + i));
    }
}

PackedData::~PackedData()
{
}

void PackedData::operator += (PackedData new_data)
{
    m_size += new_data.get_full_size();
    for(char elem : new_data.get_full_data())
    {
        m_data.push_back(elem);
    }
}

void PackedData::operator += (std::string new_data)
{
    DataSize new_size = new_data.size();
    m_size += new_size + sizeof(DataSize);
    
    for(DataSize i = 0; i < sizeof(DataSize); i++)
    {
        m_data.push_back(*((char*)&new_size + i));
    }
    for(char elem : new_data)
    {
        m_data.push_back(elem);
    }
}

PackedData PackedData::take()
{
    DataSize packet_size;
    if(m_size < sizeof(packet_size))
    {
        error("[PackedData::take()] Can't get the packet size. The data is too short.");
    }
    packet_size = *(DataSize*)m_data.data();
    if(packet_size > m_size - sizeof(packet_size))
    {
        error("[PackedData::take()] The data is shorter than the packet size.");
    }
    for(DataSize i = 0; i < sizeof(packet_size); i++)
    {
        m_data.erase(m_data.begin());
    }
    
    std::vector<char> packet_data;
    for(DataSize i = 0; i < packet_size; i++)
    {
        packet_data.push_back(m_data.front());
        m_data.erase(m_data.begin());
    }
    
    m_size -= packet_size + sizeof(DataSize);
    
    return PackedData(packet_data);
}

DataSize PackedData::get_size()
{
    return m_size;
}

DataSize PackedData::get_full_size()
{
    return m_size + sizeof(m_size);
}

std::vector<char>& PackedData::get_data()
{
    return m_data;
}

std::vector<char> PackedData::get_full_data()
{
    std::vector<char> full_data;
    
    // Преобразование m_size в массив байтов
    for(int i = 0; i < sizeof(m_size); i++)
    {
        full_data.push_back(*(((char*) &m_size) + i));
    }
    
    for(char elem : m_data)
    {
        full_data.push_back(elem);
    }
    
    return full_data;
}

}
