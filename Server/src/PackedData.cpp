//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Server 4                               |  //
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
        
        m_readonly = false;
        m_keep_data = false;
}

PackedData::PackedData(char* data, char* alloc_data, DataSize size)
{
        m_data = data;
        m_alloc_data = alloc_data;
        m_size = size;
        m_alloc_size = -1;

        m_readonly = true;
        m_keep_data = true;
}

PackedData::PackedData(void* data, DataSize size)
{
        m_alloc_data = (char*)malloc(size);

        memcpy(m_alloc_data, data, size);

        m_data = m_alloc_data;
        m_size = m_alloc_size = size;

        m_readonly = false;
        m_keep_data = false;
}

PackedData::~PackedData()
{
        if(!m_keep_data)
        {
                free(m_alloc_data);
        }
}

void PackedData::operator += (PackedData other)
{
        // Проверяем, можно ли добавлять данные в пакет
        if(m_readonly)
        {
                error("[PackedData::operator +=] Attempting to append to read-only data");
                return;
        }

        // Проверяем, выделена ли память
        if(m_alloc_data != nullptr)
        {
                // Если да, то расширяем участок памяти. Учитывается
                // размер добавляемого пакета и размер значения его размер.
                m_alloc_data = (char*) realloc(m_alloc_data, m_alloc_size + other.size() + sizeof(DataSize));
                
                // Обновляем указатель на непрочитанные данные
                m_data = m_alloc_data + m_alloc_size - m_size;
        } else {
                // Если нет, то инициализируем данные
                m_data = m_alloc_data = (char*) malloc(other.size() + sizeof(DataSize));
                m_alloc_size = 0;
        }

        // Добавляем в начало нового участка значение размера добавляемого
        // пакета
        DataSize size = other.size();
        memcpy(m_alloc_data + m_alloc_size, &size, sizeof(DataSize));

        // Добавляем данные пакета
        memcpy(m_alloc_data + m_alloc_size + sizeof(DataSize), other.data(), other.size());

        // Обновляем значение размеров хранимых данных и выделенной памяти
        m_size += other.size() + sizeof(DataSize);
        m_alloc_size += other.size() + sizeof(DataSize);
}

void PackedData::operator += (std::string string)
{
    PackedData string_data((void*) string.c_str(), string.size() + 1);
    string_data.set_keep_data(true);
    *this += string_data;
    string_data.clear();
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
        // Проверяем, можно ли освободить память
        if(m_readonly)
        {
                error("[PackedData::clear] Can't clear read-only pack.");
                return;
        }

        free(m_alloc_data);
}

void PackedData::set_keep_data(bool keep_data)
{
        if(m_readonly && (!keep_data))
        {
                error("[PackedData::set_keep_data] Can't set keep_data to false for a read-only pack.");
        }
        m_keep_data = keep_data;
}

}
