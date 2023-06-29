//
//  main.cpp
//  Server
//
//  Created by Fedor Fedor on 02.04.2023.
//

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <fcntl.h>

#include "GameManager.hpp"
#include "Event.hpp"
#include "Components/Transform.hpp"
#include "Components/Sprite.hpp"
#include "Components/Collider.hpp"
#include "Systems/MoveSystem.hpp"

#include "sys/InetNetworker.hpp"

using namespace engine;

class TestSystem : public ISystem
{
public:
	void start(GameManager *game_manager)
	{
		m_game = game_manager;
	}

	void tick()
	{
        std::vector<Client> clients = m_game->get_clients();

        for(int i = 0; i < clients.size(); i++)
        {
            auto obj = m_game->get_root()->get_child("Player_" + std::to_string(i));
            std::shared_ptr<Transform> transform = obj->get_component<Transform>();

            std::shared_ptr<Controller> controller = m_game->get_controller(clients[i].id);
            
            int player_speed = 15;
            if(controller->get_state('w'))
            {
                transform->motion.y = -player_speed;
            } else if(controller->get_state('s'))
            {
                transform->motion.y = player_speed;
            } else {
                transform->motion.y = 0;
            }

            if(controller->get_state('a'))
            {
                transform->motion.x = -player_speed;
            } else if(controller->get_state('d'))
            {
                transform->motion.x = player_speed;
            } else {
                transform->motion.x = 0;
            }
        }
	}

private:
	GameManager *m_game;
};

void move_listener(Vec2Field pos)
{
    // std::cout << "MOVE EVENT: " << pos.x << ' ' << pos.y << '\n';
}

int game_test()
{
    GameManager game;
    
    std::shared_ptr<MoveSystem> move_system = std::make_shared<MoveSystem>();
    game.add_system(move_system);
    game.add_system(std::make_shared<TestSystem>());
    
    game.get_root()->set_name("Root");

    {
        auto background = game.add_object("Background");
        auto transform = background->add_component<Transform>();
        transform->pos = { 0, 0 };
        auto sprite = background->add_component<Sprite>();
        sprite->name.s() = "sprites/level_1.png";
        sprite->size = { 800, 600 };
        sprite->pos = transform->pos;
    }

    for(int i = 0; i < 2; i++)
    {
        auto box = game.add_object("Player_" + std::to_string(i));

        auto box_transform = box->add_component<Transform>();
        box_transform->pos.x = 100 + 50 * (i % 10);
        box_transform->pos.y = 500 + 10 * (i / 10);

        auto box_collider = box->add_component<Collider>();
        box_collider->rect = { 50, 50 };

        auto sprite = box->add_component<Sprite>();
        sprite->pos = box_transform->pos;
        sprite->size = { 50, 50 };
        sprite->name.s() = "player_" + std::to_string(i % 2 + 1) + ".png";
    }

    {
        std::ifstream map_file;
        map_file.open("level_1.map");

        while(true)
        {
            std::string type;
            map_file >> type;
            if(type[0] == '#')
            {
                continue;
            } else if(type == "[END]")
            {
                break;
            } else if(type == "[WALL]")
            {
                int x, y;

                auto wall = game.add_object();

                auto wall_transform = wall->add_component<Transform>();
                
                map_file >> x >> y;
                wall_transform->pos = { x, y };

                auto wall_collider = wall->add_component<Collider>();
                wall_collider->pos = wall_transform->pos;

                map_file >> x >> y;
                wall_collider->rect = { x, y };
            } else {
                error("Unknown map object type \"" + type + "\"");
                break;
            }
        }
    }
    
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
    
    for(int i = 0; i < 100; i++)
    {
	    std::this_thread::sleep_for(std::chrono::milliseconds(5500));
	    std::cout << data.size() << std::endl;
	    networker.send_snapshot(data);
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(10000));
    return 0;
}

int main()
{
    return game_test();
}
