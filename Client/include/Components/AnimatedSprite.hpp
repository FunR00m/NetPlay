#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP

#include "Sprite.hpp"

namespace engine
{

class AnimatedSprite : public Sprite
{
public:
    AnimatedSprite();

    /// @brief Добавляет кадр
    /// @param frame_name Название текстуры кадра
    void add_frame(std::string frame_name);

    /// @return Номер текущего кадра в списке кадров
    int get_current_frame();

    /// @return Количество кадров
    int get_frame_count();
    
    /// @brief Устанавливает номер кадра
    /// @param index Номер кадра, начиная с 0
    /// @note Если index больше или равен количеству кадров, то
    /// номер кадра будет установлен на ```(index) mod (кол-во кадров)```.
    void set_frame(int index);

    /// @brief Устанавливает повторение анимации после
    /// завершения
    /// @param repeat Повторять ли анимцию
    void set_repeat(bool repeat);

    /// @brief Обновляет номер кадра
    /// @param time Текущее время
    void update(long long time);

    /// @brief Запускает анимацию
    /// @param time Текущее время
    void start(long long time);

    /// @brief Запускает анимацию в обратном порядке
    /// @param time Текущее время
    void reverse(long long time);

    /// @brief Останавливает анимацию
    void stop();

    /// @brief Устанавливает время удержания каждого кадра
    /// @param time Время удержания
    void set_frame_length(long long time);

    /// @return ```true``` если анимация запущена и не завершена, 
    /// ```false``` в противном случае
    /// @note Если анимация не повторяется и была полностью проиграна,
    /// то она останавливается.
    bool is_running();

private:
    std::vector<std::string> m_frames;
    int m_frame;
    bool m_repeat;
    bool m_running;
    long long m_start_time;
    long long m_frame_length;
    bool m_reverse;

};

}

#endif
