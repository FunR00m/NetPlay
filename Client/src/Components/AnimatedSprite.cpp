#include "Components/AnimatedSprite.hpp"

namespace engine
{

AnimatedSprite::AnimatedSprite()
{
    m_running = false;
    m_frame = 0;
    m_frame_length = 10;
    m_repeat = true;
    m_start_time = 0;
    m_reverse = false;
}

void AnimatedSprite::add_frame(std::string frame_name)
{
    m_frames.push_back(frame_name);
}

int AnimatedSprite::get_current_frame()
{
    return m_frame;
}

int AnimatedSprite::get_frame_count()
{
    return m_frames.size();
}

void AnimatedSprite::set_frame(int index)
{
    // Деление с остатком нужно, чтобы обработать
    // возможный случай index >= m_frames.size()
    m_frame = index % m_frames.size();
}

void AnimatedSprite::set_repeat(bool repeat)
{
    m_repeat = repeat;
}

void AnimatedSprite::update(long long time)
{
    if(m_running)
    {
        long long elapsed_time = time - m_start_time;
        m_frame = elapsed_time / m_frame_length;
        if(m_frame >= m_frames.size())
        {
            if(m_repeat)
            {
                m_frame = m_frame % m_frames.size();
            } else {
                m_frame = m_frames.size() - 1;
                m_running = false;
            }
        }
        
        if(m_reverse)
        {
            m_frame = m_frames.size() - m_frame - 1;
        }
    }
    
    name.s() = m_frames[m_frame];
}

void AnimatedSprite::start(long long time)
{
    m_start_time = time;
    m_running = true;
    m_frame = 0;
    m_reverse = false;
}

void AnimatedSprite::reverse(long long time)
{
    m_start_time = time;
    m_running = true;
    m_frame = m_frames.size() - 1;
    m_reverse = true;
}

void AnimatedSprite::stop()
{
    m_running = false;
}

void AnimatedSprite::set_frame_length(long long time)
{
    m_frame_length = time;
}

bool AnimatedSprite::is_running()
{
    return m_running;
}

}
