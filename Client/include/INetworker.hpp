//
//  INetworker.hpp
//  NetPlay
//
//  Created by Fedor Fedor on 15.04.2023.
//

#ifndef INetworker_h
#define INetworker_h

#include <vector>
#include <string>

#include "PackedData.hpp"

namespace engine
{

class INetworker
{
public:
    /// @brief Подключает к серверу.
    /// @param address Адрес сервера в стандартном формате
    virtual void connect(std::string address) = 0;
    
    /// @brief Отключает от сервера.
    virtual void disconnect() = 0;
    
    /// @brief Возвращает последний непрочитанный пакет данных с сервера.
    /// @return Упакованные данные пакета
    virtual PackedData receive_snapshot() = 0;

    /// @brief Отправляет пакет данных на сервер.
    /// @param snapshot Упакованные данные пакета
    virtual void send_snapshot(PackedData snapshot) = 0;
};

}

#endif /* INetworker_h */
