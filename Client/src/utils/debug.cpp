//
//  debug.cpp
//  Client
//
//  Created by Fedor Fedor on 19.04.2023.
//

#include <string>
#include <iostream>

#include "utils/debug.hpp"

void error(std::string message)
{
    std::cout << "[ERROR] " << message << '\n';
}

void warning(std::string message)
{
    std::cout << "[WARNING] " << message << '\n';
}

void fixme(std::string message)
{
    std::cout << "[FIXME] " << message << '\n';
}

void debug(std::string message)
{
    std::cout << "[DEBUG] " << message << '\n';
}
