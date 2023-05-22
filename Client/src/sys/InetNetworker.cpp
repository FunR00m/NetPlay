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
#include <strings.h>

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

void InetNetworker::send_response(PackedData response)
{
    m_responses_mtx.lock();
    m_responses.push(response);
    m_responses_size += 1;
    m_responses_mtx.unlock();
}

void InetNetworker::talk_loop()
{
    while(m_running)
    {
        // Объявляем буфер для принятых или отправляемых пакетов
        PackedData snapshot;

        // Получаем пакет с сервера
        snapshot = read_data(m_socket);

        // Проверяем, содержит ли пакет данные
        if(snapshot.size() == 0)
        {
            debug("[InetNetworker::talk_loop()] Received zero sized snapshot. Disconnecting.");
            m_running = false;
            break;
        }

        // Добавляем пакет в список непрочитанных пакетов
        m_snapshots_mtx.lock();
        m_snapshots.push(snapshot);
        m_snapshots_mtx.unlock();

        // Ждём пока появятся ответы в списке ответов
        // FIXME Изменить реализацию с помощью std::condition_variable
        // 
        while(m_responses_size == 0);

        // Копируем ответ в буфер
        m_responses_mtx.lock();
        if(m_responses.size() > 0)
        {
            m_responses_size -= 1;
            snapshot = m_responses.front();
            m_responses.pop();
        }
        m_responses_mtx.unlock();

        // Отправляем ответ
        send_data(m_socket, snapshot);

        // Удаляем данные ответа
        snapshot.clear();
    }

    // Закрываем сокет после завершения цикла
    close(m_socket);
}

void InetNetworker::send_data(int socket, PackedData data)
{
    char *raw_data = data.data();
    DataSize length = data.size();
    
    // Отправка длины сообщения
    write(socket, &length, sizeof(length));
    
    // Отправка сообщения
    const int buffer_size = BUFFER_SIZE;
    for(long long i = 0; i + buffer_size < length; i += buffer_size)
    {
	// Последовательно отправляем куски данных размером buffer_size
        write(socket, raw_data + i, buffer_size);
    }
    // Отправляем оставшиеся данные
    write(socket, raw_data + length / buffer_size * buffer_size, length % buffer_size);
}

PackedData InetNetworker::read_data(int socket)
{
    DataSize length;

    // Принимаем размер читаемых данных
    if(read(socket, &length, sizeof(length)) == 0)
    {
        return PackedData();
    }

    const int buffer_size = BUFFER_SIZE;

    // Выделяем память для принятых данных
    char *raw_data = (char*) malloc(length);
    
    for(long long i = 0; i + buffer_size < length; i+= buffer_size)
    {
	// Читаем кусок данных длиной buffer_size и добавляем его raw_data
        read(socket, raw_data + i, buffer_size);
    }
    // Читаем оставшиеся данные
    read(socket, raw_data + length / buffer_size * buffer_size, length % buffer_size);
    
    // Создаём пакет из сырых данных. Эти данные будут скопированы.
    PackedData data(raw_data, length);

    // Освобождаем выделенную память
    free(raw_data);
    return data;
}

}
}
