//
//  INetworker.hpp
//  NetPlay
//
//  Created by Fedor Fedor on 15.04.2023.
//

#ifndef INetworker_h
#define INetworker_h

#include <vector>

#include "PackedData.hpp"
#include "Client.hpp"

namespace engine
{

class INetworker
{
public:
    virtual void start_listening() = 0;
    
    virtual void stop() = 0;
    
    /// Отправляет снимок клиентам
    /// - Parameter snapshot: Упакованные данные снимка
    ///
    virtual void send_snapshot(PackedData snapshot) = 0;
    
    virtual PackedData get_data(long long client_id) = 0;
    
    virtual std::vector<Client> get_clients() = 0;
    
    virtual void kick_client(long long client_id) = 0;
};

}

#endif /* INetworker_h */
