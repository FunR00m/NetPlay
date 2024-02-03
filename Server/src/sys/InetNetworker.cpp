//
//  InetNetworker.cpp
//  Server
//
//  Created by Fedor Fedor on 15.04.2023.
//

#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

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
    m_max_client_id = 0;
    m_clients_count = 0;
    m_max_snapshot_id = 0;
}

void InetNetworker::start_listening()
{
    // Проверяем, не запущен ли модуль
    if(m_running)
    {
        error("[InetNetworker::start_listening()] Already running. Can't start again.");
        return;
    }

    // Создаём сокет для новых подключений
    m_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_listen_socket == -1)
    {
        error("[InetNetworker::start_listening()] Failed to create a socket.");
        return;
    }
    
    // Определяем адрес сервера
    sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    
    int port = 8001;
    debug("Port: " + std::to_string(port));
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_family = AF_INET;
    
    // Соединяем сокет с адресом сервера
    if(bind(m_listen_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1)
    {
        error("[InetNetworker::start_listening()] Failed to bind the socket.");
        return;
    }
    
    // Начинаем прослушивать сокет
    if(listen(m_listen_socket, 4) == -1)
    {
        error("[InetNetworker::start_listening()] Faield to start listening the socket.");
        return;
    }
    
    m_running = true;
    
    // Запускаем поток обработки новых подключений
    m_listen_thread = std::make_shared<std::thread>(&InetNetworker::listen_thread, this);
}

void InetNetworker::stop()
{
    if(m_running)
    {
        m_running = false;
    } else {
        warning("[InetNetworker::stop()] Attempting to stop but it's not running.");
    }
}

void InetNetworker::send_snapshot(PackedData snapshot)
{ 
    // Проверяем, есть ли подключенные клиенты.
    if(m_clients_count == 0)
    {
        // Если нет, то удаляем пакет, потому что его некому отправлять
        snapshot.clear();
        return;
    }


    m_snapshot_to_data_mtx.lock();
    
    // Записываем новый пакет в индекс
    m_snapshot_to_data[m_max_snapshot_id].first = m_clients_count;
    m_snapshot_to_data[m_max_snapshot_id].second = snapshot;
    
    m_snapshot_to_data_mtx.unlock();


    // Увеличиваем счётчик отправляемых пакетов
    m_max_snapshot_id += 1;
}

PackedData InetNetworker::get_response(long long client_id)
{
    m_id_to_data_mtx.lock();

    // Копируем данные из индекса
    PackedData data = m_id_to_data[client_id];
    
    // Очищаем данные в индексе
    m_id_to_data[client_id] = PackedData();

    m_id_to_data_mtx.unlock();
    
    return data;
}

std::vector<Client> InetNetworker::get_clients()
{
    std::lock_guard<std::mutex> lock(m_clients_mtx);
    return m_clients;
}

void InetNetworker::kick_client(long long client_id)
{
    fixme("InetNetworker::kick_client stub");
}

void InetNetworker::listen_thread()
{
    while(m_running)
    {
        sockaddr_in client_addr;
        socklen_t client_len;

        // Принимаем подключение
        int client_socket = accept(m_listen_socket, (sockaddr*)&client_addr, &client_len);
        
        // Увеличиваем счётчик номеров клиентов
        m_max_client_id += 1;

        // Увеличиваем счётчик количества подключенных клиентов
        m_clients_count += 1;
        
        m_clients_mtx.lock();

        // Добавляем клиента в список клиентов
        m_clients.push_back({ m_max_client_id });

        // Запускаем поток для нового клиента
        m_id_to_thread[m_max_client_id] = std::make_shared<std::thread>(&InetNetworker::client_thread, this, client_socket, m_clients.back());

        m_clients_mtx.unlock();
    }
}

void InetNetworker::client_thread(int socket, Client client)
{
    debug("Client connected!");

    // Устанавливаем номер последнего отправленного пакета
    unsigned long long last_sent_snapshot_id = m_max_snapshot_id;
    while(m_running)
    {
        // Ждём появления пакетов для отправки
        while(last_sent_snapshot_id == m_max_snapshot_id);

        // Копируем пакет из индекса
        m_snapshot_to_data_mtx.lock();
        PackedData snapshot_data = m_snapshot_to_data[last_sent_snapshot_id + 1].second;
        m_snapshot_to_data_mtx.unlock();
        

        if(snapshot_data.size() == 0)
        {
            continue;
        }
        
        // Отправляем пакет
        send_data(socket, snapshot_data);
        

        m_snapshot_to_data_mtx.lock();

        // Читаем счётчик клиентов, которые получили данный пакет
        auto &ready_count = m_snapshot_to_data[last_sent_snapshot_id + 1].first;

        // Уменьшаем счётчик
        ready_count -= 1;

        // Проверяем, остались ли клиенты, не получившие пакет
        if(ready_count == 0)
        {
            // Если пакет больше не нужен, удаляем его.
            m_snapshot_to_data[last_sent_snapshot_id + 1].second.clear();
            m_snapshot_to_data.erase(last_sent_snapshot_id + 1);
        }

        m_snapshot_to_data_mtx.unlock();

        
        // Увеличиваем номер последнего отправленного пакета
        last_sent_snapshot_id += 1;

        // Получаем ответ клиента
        PackedData data = read_data(socket);

        // Проверяем, отключился ли клиент
        // FIXME Изменить признак отключения клиента. Он может отправить действительный
        // пакет нулевой длины, не отключаясь.
        //
        if(data.size() == 0)
        {
            // Закрываем соединение
            close(socket);
            remove_client(client.id);
            data.clear();
            return;
        }

        m_id_to_data_mtx.lock();

        // Записываем полученный ответ в соответствующую ячейку индекса
        m_id_to_data[client.id] = data;

        m_id_to_data_mtx.unlock();
    }
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
    
    // Создаём пакет из сырых данных
    PackedData data(raw_data, length);
    data.set_keep_data(true);

    // Освобождаем выделенную память
    free(raw_data);

    return data;
}

void InetNetworker::remove_client(long long client_id)
{
    // Номер клиента в массиве
    long long i = 0;
    

    m_clients_mtx.lock();
    
    // Ищем номер клиента в массиве
    for(i = 0; i < m_clients.size(); i++)
    {
        if(m_clients[i].id == client_id)
        {
            break;
        }
    }

    // Находим итератор клиента
    auto client = m_clients.begin() + i;
    
    // Проверяем существует ли клиент с таким номером
    if(client == m_clients.end())
    {
        return;
    }

    // Удаляем клиента из массива
    m_clients.erase(client);
    
    m_clients_mtx.unlock();

    
    // Уменьшаем счётчик подключенных клиентов
    m_clients_count -= 1;

    debug("Client disconnected");
}

}
}
