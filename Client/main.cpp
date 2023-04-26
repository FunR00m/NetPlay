//
//  main.cpp
//  Client
//
//  Created by Fedor Fedor on 18.04.2023.
//

#include <iostream>
#include <chrono>

#include "GameManager.hpp"
#include "sys/InetNetworker.hpp"

using namespace engine;

int network_test()
{
    sys::InetNetworker networker;
    networker.connect("localhost:8001");
    
    PackedData mail;
    mail += "hello";
    for(int i = 0; i < 1000; i++)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        PackedData data = networker.receive_snapshot();
        std::cout << data.get_size() << std::endl;
        if(data.get_size() > 0)
        {
            for(char c : data.get_data())
            {
                std::cout << c;
            }
            std::cout << '\n';
        }
        networker.send_snapshot(mail);
    }


    std::this_thread::sleep_for(std::chrono::seconds(1000));
    networker.disconnect();

    return 0;
}
int main(int argc, const char * argv[]) {
    // GameManager game_manager;
    // auto box = game_manager.add_object();
    // box->set_name("Box");
    
    // std::cout << game_manager.get_root()->get_child("Box")->get_name() << std::endl;

    return network_test();
}
