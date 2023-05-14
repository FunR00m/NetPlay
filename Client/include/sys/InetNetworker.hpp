//
//  CInetNetworker.hpp
//  Client
//
//  Created by Fedor Fedor on 23.04.2023.
//

#ifndef INETNETWORKER_HPP
#define INETNETWORKER_HPP

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

    /// @brief Подключается к серверу по данному адресу
    /// @param address Адрес сервера
    /// @note Чтение адреса не реализовано. Подключается к
    /// localhost:8001
    void connect(std::string address) override;
    
    /// @brief Отключает сервера
    void disconnect() override;

    /// @return Возвращает самый старый непрочитанный снимок
    PackedData receive_snapshot() override;
    
    /// @brief Добавляет ответ в очередь
    /// @param response Упакованные данные ответа
    void send_response(PackedData response) override;

private:
    std::atomic_bool m_running;

    /// @brief Очередь из принятых снимков
    std::queue<PackedData> m_snapshots;
    std::mutex m_snapshots_mtx;

    /// @brief Очередь из ответов, ждущих отправления на сервер
    std::queue<PackedData> m_responses;
    std::mutex m_responses_mtx;

    /// @brief Количество ответов
    std::atomic_int m_responses_size;

    /// @brief Сокет взаимодействия с сервером
    int m_socket;

    /// @brief Поток взаимодействия с сервером
    std::shared_ptr<std::thread> m_talk_thread;

    /// @brief Главная функция, взаимодействующая с сервером.
    void talk_loop();

    /// @brief Отправляет данные на сокет
    /// @param socket Сокет, на который нужно отправить данные
    /// @param data Отправляемые данные
    void send_data(int socket, PackedData data);
    
    /// @brief Принимает данные с сокета
    /// @param socket Сокет, с которого нужно прочитать данные
    /// @return Принятые данные
    PackedData read_data(int socket);
    
};

}
}

#endif
