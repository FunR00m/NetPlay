//
//  main.cpp
//  Server
//
//  Created by Fedor Fedor on 02.04.2023.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <fcntl.h>

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

class MoveSystem : public ISystem
{
public:
    void start(GameManager *game_manager) override
    {
        m_game_manager = game_manager;
    }
    
    void tick() override
    {
        for(auto obj : m_game_manager->get_objects())
        {
            auto transform = obj->get_component<Transform>();
            if(transform)
            {
                transform->pos += transform->motion;
                transform->pos = { 1, 2 };
                transform->move_event.invoke(transform->pos);
                
                std::cout << obj->get_name() + ": " << transform->pos.x << ' ' << transform->pos.y << '\n';
            }
        }
    }

private:
    GameManager *m_game_manager;
};

void move_listener(Vec2Field pos)
{
    std::cout << "MOVE EVENT: " << pos.x << ' ' << pos.y << '\n';
}

int game_test()
{
    GameManager game;
    
    std::shared_ptr<MoveSystem> move_system = std::make_shared<MoveSystem>();
    game.add_system(move_system);
    
    auto box = game.add_object("Box");
    auto box_transform = box->add_component<Transform>();
    box_transform->move_event += move_listener;
    
    box_transform->pos.x = 10;
    box_transform->pos.y = 20;
    box_transform->motion.x = 5;
    
    game.start();
    
    return 0;
}

int network_test()
{
    sys::InetNetworker networker;
    networker.start_listening();
    
    char buffer[2402];
    bzero(buffer, 2402);
    int file = open("/Users/fedor/Programs/NetPlay/Server/test", O_RDONLY);
    read(file, buffer, 2402);
    close(file);
    
    PackedData file_data(buffer, 2402);
    
    PackedData data;
    data += "Hello, World!";
    data += "12345";
    data += "6789";
    data += "yeah yeah";
    
    for(int i = 0; i < 100; i++)
    {
	    std::this_thread::sleep_for(std::chrono::milliseconds(5500));
	    std::cout << data.get_size() << std::endl;
	    networker.send_snapshot(data);
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(10000));
    return 0;
}

int main()
{
    return game_test();
}
