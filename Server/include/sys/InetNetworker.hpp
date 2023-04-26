//
//  InetNetworker.hpp
//  Server
//
//  Created by Fedor Fedor on 15.04.2023.
//

#ifndef InetNetworker_hpp
#define InetNetworker_hpp

#include <thread>
#include <map>
#include <mutex>
#include <atomic>
#include <memory>

#include "INetworker.hpp"

namespace engine
{
namespace sys
{

class InetNetworker : public INetworker
{
public:
    InetNetworker();
    
    void start_listening() override;
    
    void stop() override;
    
    /// Отправляет снимок клиентам
    /// - Parameter snapshot: Упакованные данные снимка
    ///
    void send_snapshot(PackedData snapshot) override;
    
    PackedData get_data(long long client_id) override;
    
    std::vector<Client> get_clients() override;
    
    void kick_client(long long client_id) override;
    
private:
    std::atomic_bool m_running;
    
    int m_listen_socket;
    
    std::atomic_int m_max_client_id;
    
    std::atomic_int m_clients_count;
    
    std::vector<Client> m_clients;
    std::mutex m_clients_mtx;
    
    std::map<long long, std::shared_ptr<std::thread>> m_id_to_thread;
    
    /// Словарь вида *номер кадра - пара*.
    /// Пара содержит количество клиентов, которым НЕ был отправлен снимок, и упакованные
    /// данные снимка.
    std::map<unsigned long long, std::pair<int, PackedData>> m_snapshot_to_data;
    std::mutex m_snapshot_to_data_mtx;
    
    std::map<long long, PackedData> m_id_to_data;
    std::mutex m_id_to_data_mtx;
    
    std::atomic_int64_t m_max_snapshot_id;
    
    std::shared_ptr<std::thread> m_listen_thread;
    
    void listen_thread();
    
    void client_thread(int socket, Client client);
    
    void send_data(int socket, PackedData data);
    
    void remove_client(long long client_id);
    
    PackedData read_data(int socket);
};

}
}

#endif /* InetNetworker_hpp */
