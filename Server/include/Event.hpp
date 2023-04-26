//
//  Event.hpp
//  Server
//
//  Created by Fedor Fedor on 12.04.2023.
//

#ifndef Event_hpp
#define Event_hpp

#include <vector>
#include <functional>

namespace engine
{

template<typename... Args>
class Event
{
public:
    void invoke(Args... args)
    {
        for(auto listener : m_listeners)
        {
            listener(args...);
        }
    }
    
    void operator += (std::function<void(Args...)> listener)
    {
        m_listeners.push_back(listener);
    }
    
private:
    std::vector<std::function<void(Args...)>> m_listeners;
};

}

#endif /* Event_hpp */
