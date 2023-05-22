//  __________-----------================-----------__________  //
//  =====================<  NETPLAY  4  >=====================  //
//  | Проект: NetPlay Server 4                               |  //
//  | Файл: INetworker.hpp                                   |  //
//  | Автор: Fedor Buben <bubenfedor0@gmail.com>             |  //
//  | Дата создания: 19.04.2023                              |  //
//  | Дата изменения: 11.05.2023                             |  //
//  | Описание: Интерфейс взаимодействия сервера с клиентами |  //
//  |--------------------------------------------------------|  //
//  | ПОДРОБНОЕ ОПИСАНИЕ                                     |  //
//  |   Данный интерфейс предоставляет функционал для        |  //
//  | взаимодействия между сервером и клиентами. Реализации  |  //
//  | могут использовать разные протоколы, например TCP или  |  //
//  | Bluetooth.                                             |  //
//  |                                                        |  //
//  ==========================================================  //
//                                                              //

#ifndef INetworker_hpp
#define INetworker_hpp

#include <vector>

#include "PackedData.hpp"
#include "Client.hpp"

namespace engine
{

/// @brief Интерфейс модуля удалённого взаимодействия с клиентами
class INetworker
{
public:
    /// @brief Начинает принимать подключения
    virtual void start_listening() = 0;
    
    /// @brief Прекращает принимать подключения и отключает всех клиентов
    virtual void stop() = 0;
    
    /// @brief Отправляет снимок всем клиентам
    /// @param snapshot Упакованные данные снимка
    virtual void send_snapshot(PackedData snapshot) = 0;
    
    /// @brief Возвращает последний принятый ответ данного клиента
    /// @param client_id Номер клиента
    /// @return Упакованные данные ответа
    virtual PackedData get_response(long long client_id) = 0;
    
    /// @return Список подключенных клиентов
    virtual std::vector<Client> get_clients() = 0;
    
    /// @brief Принудительно отключает клиента
    /// @param client_id Номер клиента
    virtual void kick_client(long long client_id) = 0;
};

}

#endif
