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
    
    std::this_thread::sleep_for(std::chrono::seconds(8));
    PackedData data = networker.receive_snapshot();

    std::cout << data.get_size() << std::endl;


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
