//
//  main.cpp
//  Client
//
//  Created by Fedor Fedor on 18.04.2023.
//

#include <iostream>
#include <chrono>
#include <thread>

#include "GameManager.hpp"
#include "Event.hpp"
#include "sys/InetNetworker.hpp"

using namespace engine;

class Transform : public IComponent
{
public:
    Vec2Field pos;
    Vec2Field motion;
    
    Event<Vec2Field> move_event;
    
    Transform()
    {
        pos = { 0, 0 };
        motion = { 0, 0 };
    }

    PackedData pack() override
    {
        PackedData data;
        data += pos.pack();
        data += motion.pack();
        return data;
    }
    
    PackedData fetch_changes() override
    {
        PackedData data;
        data += pos.fetch_changes();
        data += motion.fetch_changes();
        return data;
    }

    void unpack(PackedData data) override
    {
        pos.unpack(data.take());
        motion.unpack(data.take());
    }

    void apply_changes(PackedData data) override
    {
        pos.apply_changes(data.take());
        motion.apply_changes(data.take());
    }
};

class TestSystem : public ISystem
{
public:
    void start(GameManager *game_manager)
    {
        m_game_manager = game_manager;
    }

    void tick()
    {
        for(auto obj : m_game_manager->get_objects())
        {
            auto transform = obj->get_component<Transform>();
            if(transform)
            {
                debug("Wow! Transform!");
                debug(std::to_string(transform->pos.x));
            }
        }
    }

private:
    GameManager *m_game_manager;
};

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
        networker.send_response(mail);
    }


    std::this_thread::sleep_for(std::chrono::seconds(1000));
    networker.disconnect();

    return 0;
}

int game_test()
{
    GameManager game_manager;
    game_manager.register_component<Transform>();
    game_manager.add_system(std::make_shared<TestSystem>());
    game_manager.start();

    return 0;
}
int main(int argc, const char * argv[]) {
    // GameManager game_manager;
    // auto box = game_manager.add_object();
    // box->set_name("Box");
    
    // std::cout << game_manager.get_root()->get_child("Box")->get_name() << std::endl;

    return game_test();
}
