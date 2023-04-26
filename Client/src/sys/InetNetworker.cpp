//
//  InetNetworker.cpp
//  Client
//
//  Created by Fedor Fedor on 23.04.2023.
//

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "sys/InetNetworker.hpp"
#include "utils/debug.hpp"

#define BUFFER_SIZE 1024

namespace engine
{
namespace sys
{

InetNetworker::InetNetworker()
{
    m_running = false;
}

void InetNetworker::connect(std::string address)
{
    if(m_running)
    {
        warning("[InetNetworker::connect] Already connected to a server. Disconnecting.");
        disconnect();
    }

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket == -1)
    {
        error("[InetNetworker::connect] Failed to create a socket.");
        return;
    }

    sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_port = htons(8001);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_family = AF_INET;

    if(::connect(m_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1)
    {
        error("[InetNetworker::connect] Failed to connect to the server.");
        return;
    }

    m_running = true;
    m_talk_thread = std::make_shared<std::thread>(&InetNetworker::talk_loop, this);
}

void InetNetworker::disconnect()
{
    m_running = false;
    m_talk_thread->join();
}

PackedData InetNetworker::receive_snapshot()
{
    std::lock_guard<std::mutex> lock(m_snapshots_mtx);
    if(m_snapshots.size() > 0)
    {
        PackedData snapshot = m_snapshots.front();
        m_snapshots.pop();
        return snapshot;
    } else {
        return PackedData();
    }
}

void InetNetworker::send_snapshot(PackedData snapshot)
{
    m_client_snapshots_mtx.lock();
    m_client_snapshots.push(snapshot);
    m_client_snapshots_size += 1;
    m_client_snapshots_mtx.unlock();
}

void InetNetworker::talk_loop()
{
    while(m_running)
    {
        PackedData snapshot = read_data(m_socket);
        if(snapshot.get_size() == 0)
        {
            m_running = false;
            break;
        }

        m_snapshots_mtx.lock();
        // debug("SNAPSHOT");
        m_snapshots.push(snapshot);
        m_snapshots_mtx.unlock();

        while(m_client_snapshots_size == 0);
        m_client_snapshots_mtx.lock();
        if(m_client_snapshots.size() > 0)
        {
            m_client_snapshots_size -= 1;
            snapshot = m_client_snapshots.front();
            m_client_snapshots.pop();
        }
        m_client_snapshots_mtx.unlock();

        send_data(m_socket, snapshot);
    }
}

void InetNetworker::send_data(int socket, PackedData data)
{
    char *raw_data = data.get_data().data();
    DataSize length = data.get_size();
    
    // Отправка длины сообщения
    write(socket, &length, sizeof(length));
    
    // Отправка сообщения
    const int buffer_size = BUFFER_SIZE;
    for(long long i = 0; i + buffer_size < length; i += buffer_size)
    {
        write(socket, raw_data + i, buffer_size);
    }
    write(socket, raw_data + length / buffer_size * buffer_size, length % buffer_size);
}

PackedData InetNetworker::read_data(int socket)
{
    DataSize length;
    if(read(socket, &length, sizeof(length)) == 0)
    {
        return PackedData();
    }

    const int buffer_size = BUFFER_SIZE;
    char *raw_data = (char*) malloc(length);
    
    for(long long i = 0; i + buffer_size < length; i+= buffer_size)
    {
        read(socket, raw_data + i, buffer_size);
    }
    read(socket, raw_data + length / buffer_size * buffer_size, length % buffer_size);
    
    PackedData data(raw_data, length);
    free(raw_data);
    return data;
}

}
}
