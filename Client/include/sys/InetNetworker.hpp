//
//  CInetNetworker.hpp
//  Client
//
//  Created by Fedor Fedor on 23.04.2023.
//

#ifndef InetNetworker_hpp
#define InetNetworker_hpp

#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
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

    void connect(std::string address) override;
    
    void disconnect() override;

    PackedData receive_snapshot() override;
    
    void send_snapshot(PackedData snapshot) override;

private:
    std::atomic_bool m_running;

    std::queue<PackedData> m_snapshots;
    std::mutex m_snapshots_mtx;

    std::queue<PackedData> m_client_snapshots;
    std::mutex m_client_snapshots_mtx;

    std::atomic_int m_client_snapshots_size;

    int m_socket;

    std::shared_ptr<std::thread> m_talk_thread;

    /// @brief Главная функция, взаимодействующая с сервером.
    void talk_loop();

    void send_data(int socket, PackedData data);

    PackedData read_data(int socket);
    
};

}
}

#endif /* InetNetworker_hpp */
