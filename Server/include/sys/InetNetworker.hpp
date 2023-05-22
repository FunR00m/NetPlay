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

/// @brief Модуль удалённого взаимодействия с клиентами по Интернету
class InetNetworker : public INetworker
{
public:
    InetNetworker();
    
    /// @brief Начинает принимать подключения
    void start_listening() override;
    
    /// @brief Прекращает принимать подключения и отключает всех клиентов
    void stop() override;
    
    /// @brief Отправляет снимок всем клиентам
    /// @param snapshot Упакованные данные снимка
    void send_snapshot(PackedData snapshot) override;
    
    /// @brief Возвращает последний принятый ответ данного клиента
    /// @param client_id Номер клиента
    /// @return Упакованные данные ответа
    PackedData get_response(long long client_id) override;
    
    /// @return Список подключенных клиентов
    std::vector<Client> get_clients() override;
    
    /// @brief Принудительно отключает клиента
    /// @param client_id Номер клиента
    void kick_client(long long client_id) override;
    
private:
    /// Запущен ли модуль
    std::atomic_bool m_running;
    
    /// Сокет, ожидающий подключения
    int m_listen_socket;
    
    /// Максимальный использованный номер клиента
    std::atomic_int m_max_client_id;
    
    /// Количество подключенных клиентов
    std::atomic_int m_clients_count;
    
    /// Список подключенных клиентов
    std::vector<Client> m_clients;
    std::mutex m_clients_mtx;
    
    /// Словарь вида номер клиента - указатель на его поток
    std::map<long long, std::shared_ptr<std::thread>> m_id_to_thread;
    
    /// Словарь вида номер кадра - пара.
    /// Пара содержит количество клиентов, которым НЕ был отправлен снимок, и упакованные
    /// данные снимка.
    std::map<unsigned long long, std::pair<int, PackedData>> m_snapshot_to_data;
    std::mutex m_snapshot_to_data_mtx;
    
    /// Словарь вида номер клиента - последний принятый ответ
    std::map<long long, PackedData> m_id_to_data;
    std::mutex m_id_to_data_mtx;
    
    /// Номер последнего снимка
    std::atomic_int64_t m_max_snapshot_id;
    
    /// Указатель на поток, принимающий новые подключения
    std::shared_ptr<std::thread> m_listen_thread;
    
    /// @brief Функция, принимающая новые подключения
    void listen_thread();
    
    /// @brief Функция взаимодействия с клиентом
    /// @param socket Сокет клиента
    /// @param client Данные о клиенте
    void client_thread(int socket, Client client);
    
    /// @brief Отправляет данные на сокет
    /// @param socket Сокет, на который нужно отправить данные
    /// @param data Отправляемые данные
    void send_data(int socket, PackedData data);
    
    /// @brief Принимает данные с сокета
    /// @param socket Сокет, с которого нужно прочитать данные
    /// @return Принятые данные
    PackedData read_data(int socket);
    
    /// @brief Удаляет клиента из индекса
    /// @param client_id Номер клиента
    void remove_client(long long client_id);
};

}
}

#endif /* InetNetworker_hpp */
