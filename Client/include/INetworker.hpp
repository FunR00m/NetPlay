//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Client 4                               |  //
//  | Файл: INetworker.hpp                                   |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 19.04.2023                              |  //
//  | Дата изменения: 12.05.2023                             |  //
//  | Описание: Интерфейс взаимодействия клиента с сервером  |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  |   Данный интерфейс предоставляет функционал для        |  //
//  | взаимодействия между клинетом и сервером. Реализации   |  //
//  | могут использовать разные протоколы, например TCP или  |  //
//  | Bluetooth.                                             |  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

#ifndef INetworker_hpp
#define INetworker_hpp

#include <vector>
#include <string>

#include "PackedData.hpp"

namespace engine
{

/// @brief Интерфейс модуля удалённого взаимодействия с клиентами
class INetworker
{
public:
    /// @brief Подключает к серверу.
    /// @param address Адрес сервера в стандартном формате
    virtual void connect(const std::string& address) = 0;
    
    /// @brief Отключает от сервера.
    virtual void disconnect() = 0;
    
    /// @brief Возвращает последний непрочитанный пакет данных с сервера.
    /// @return Упакованные данные пакета
    virtual PackedData receive_snapshot() = 0;

    /// @brief Отправляет пакет данных на сервер.
    /// @param response Упакованные данные пакета
    virtual void send_response(PackedData response) = 0;
};

}

#endif
