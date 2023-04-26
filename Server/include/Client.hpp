//
//  Client.hpp
//  NetPlay
//
//  Created by Fedor Fedor on 15.04.2023.
//

#ifndef IClient_h
#define IClient_h

#include <string>

namespace engine
{

struct Client
{
    long long id;
    std::string address;
};

}

#endif /* IClient_h */
