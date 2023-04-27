//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Server 4                               |  //
//  | Файл: PackedData.hpp                                   |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 02.04.2023                              |  //
//  | Дата изменения: 02.04.2023                             |  //
//  | Описание: Последовательность байт, готовая к передаче. |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

#ifndef PackedData_hpp
#define PackedData_hpp

#include <vector>
#include <string>
#include <memory>

namespace engine
{

using DataSize = unsigned long long;

class PackedData
{
public:
    PackedData();
    
    PackedData(std::vector<char> &data);
    
    PackedData(void* object, DataSize size);
    
    ~PackedData();
    
    /// Добавляет данные другого объекта PackedData.
    ///
    /// - Parameter new_data: Объект, данные которого нужно добавить
    ///
    void operator += (PackedData new_data);
    
    /// Добавляет символы данной строки к данным.
    ///
    /// - Parameter new_data: Строка, которую нужно добавить
    ///
    void operator += (std::string new_data);

    /// Удаляет первый упакованный пакет данных и возвращает его.
    PackedData take();
    
    /// Возвращает размер хранимых данных.
    DataSize get_size();
    
    /// Возвращает размер хранимых данных вместе с размером переменной, указывающей на их размер.
    DataSize get_full_size();
    
    /// Возвращает хранимые данные.
    std::vector<char>& get_data();
    
    /// Возвращает хранимые данные с их размером вначале.
    std::vector<char> get_full_data();

private:
    /// Хранимые данные
    std::vector<char> m_data;
    
    /// Размер хранимых данных
    DataSize m_size;
};

}

#endif /* PackedData_hpp */
