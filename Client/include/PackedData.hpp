//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Client 4                               |  //
//  | Файл: PackedData.hpp                                   |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 02.04.2023                              |  //
//  | Дата изменения: 22.05.2023                             |  //
//  | Описание: Последовательность байт, готовая к передаче. |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  |   Перед передачей данных их нужно преобразовать в      |  //
//  | последовательность байт. После передачи их нужно       |  //
//  | прочитать. Для этих дейтвий оптимизирован тип          |  //
//  | PackedData. С помощью него можно легко упаковать       |  //
//  | произвольные данные и распаковать их на другой стороне.|  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

#ifndef PACKEDDATA_HPP
#define PACKEDDATA_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>

namespace engine
{

using DataSize = unsigned int;

class PackedData
{
public:
        /// @brief Конструирует объект без начальных данных
        PackedData();

        /// @brief Конструирует объект, не копируя данные. При этом
        /// добавить новые данные будет нельзя.
        /// @param data Начальные данные
        /// @param alloc_data Начало выделенной памяти
        /// @param size Размер начальных данных
        PackedData(char* data, char* alloc_data, DataSize size);

        /// @brief Конструирует объект, копируя данные
        /// @param data Начальные данные
        /// @param size Размер данных
        PackedData(void* data, DataSize size);

        /// @brief Добавляет данные другого объекта PackedData.
        /// Происходит копирование.
        /// @param other Объект, данные которого нужно добавить
        void operator += (PackedData other);

        /// @brief Добавляет символы данной строки к данным.
        /// Происходит копирование. Заключающий ноль учитывается!
        /// @param string Строка, которую нужно добавить
        void operator += (std::string string);

        /// @brief Возвращает следующий пакет упакованных данных.
        /// @return Пакет упакованных данных.
        PackedData take();

        /// @return Указатель на хранимые данные
        char* data();

        /// @return Указатель на начало хранимых данных
        char* alloc_data();

        /// @return Размер хранимых данных
        DataSize size();

        /// @return Размер выделенной памяти
        DataSize alloc_size();

        /// @brief Очищает выделенные данные. Другие параметры при этом
        /// не меняются.
        void clear();

private:
        char* m_data;
        char* m_alloc_data;

        DataSize m_size;
        DataSize m_alloc_size;
};

}

#endif
