//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Client 4                               |  //
//  | Файл: PackedData.cpp                                   |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 02.04.2023                              |  //
//  | Дата изменения: 22.04.2023                             |  //
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
        m_alloc_size = 0;
        m_data = nullptr;
        m_alloc_data = nullptr;
}

PackedData::PackedData(char* data, char* alloc_data, DataSize size)
{
        m_data = data;
        m_alloc_data = alloc_data;
        m_size = size;
        m_alloc_size = -1;
}

PackedData::PackedData(void* data, DataSize size)
{
        m_alloc_data = (char*)malloc(size);

        memcpy(m_alloc_data, data, size);

        m_data = m_alloc_data;
        m_size = m_alloc_size = size;
}

void PackedData::operator += (PackedData other)
{
        if(m_alloc_size == -1)
        {
                error("[PackedData::operator +=] Attempting to change read-only data");
                return;
        }
        if(m_alloc_data != nullptr)
        {
                m_alloc_data = (char*) realloc(m_alloc_data, m_alloc_size + other.size() + sizeof(DataSize));
                m_data = m_alloc_data + m_alloc_size - m_size;
        } else {
                m_data = m_alloc_data = (char*) malloc(other.size() + sizeof(DataSize));
                m_alloc_size = 0;
        }
        DataSize size = other.size();
        memcpy(m_alloc_data + m_alloc_size, &size, sizeof(DataSize));
        memcpy(m_alloc_data + m_alloc_size + sizeof(DataSize), other.data(), other.size());

        m_size += other.size() + sizeof(DataSize);
        m_alloc_size += other.size() + sizeof(DataSize);
}

void PackedData::operator += (std::string string)
{
    PackedData string_data((void*) string.c_str(), string.size() + 1);
    *this += string_data;
}

PackedData PackedData::take()
{
        DataSize part_size;
        memcpy(&part_size, m_data, sizeof(DataSize));

        m_data += sizeof(DataSize);
        PackedData data(m_data, m_alloc_data, part_size);

        m_data += part_size;
        m_size -= part_size + sizeof(DataSize);

        return data;
}

char* PackedData::data()
{
        return m_data;
}

char* PackedData::alloc_data()
{
        return m_alloc_data;
}

DataSize PackedData::size()
{
        return m_size;
}

DataSize PackedData::alloc_size()
{
        return m_alloc_size;
}

void PackedData::clear()
{
        free(m_alloc_data);
}

}
