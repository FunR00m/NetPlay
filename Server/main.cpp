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
#include "Components/TriggerCollider.hpp"
#include "Systems/MoveSystem.hpp"

#include "Controls.hpp"

#include "sys/InetNetworker.hpp"

using namespace engine;
        
const int player_count = 2;

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

        int player_speed = 15;

        for(int i = 0; i < player_count; i++)
        {
            auto obj = m_game->get_root()->get_child("Player_" + std::to_string(i));
            std::shared_ptr<Transform> transform = obj->get_component<Transform>();

            std::shared_ptr<Controller> controller = nullptr;
            
            for(Client client : clients)
            {
                if(client.id % player_count == i)
                {
                    controller = m_game->get_controller(client.id);
                    break;
                }
            }

            if(controller == nullptr)
            {
                continue;
            }

            if(controller->get_state(C_Up))
            {
                transform->motion.y = -player_speed;
            } else if(controller->get_state(C_Down))
            {
                transform->motion.y = player_speed;
            } else {
                transform->motion.y = 0;
            }

            if(controller->get_state(C_Left))
            {
                transform->motion.x = -player_speed;
            } else if(controller->get_state(C_Right))
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

class DoorSystem : public ISystem
{
public:
	void start(GameManager *game_manager)
	{
		m_game = game_manager;
	}

    void tick()
    {
        std::vector<Client> clients = m_game->get_clients();

        auto key = m_game->get_object("Key");

        if(key == nullptr)
        {
            return;
        }

        auto key_collider = key->get_component<TriggerCollider>();

        for(int i = 0; i < player_count; i++)
        {
            auto player = m_game->get_object("Player_" + std::to_string(i));
            auto player_collider = player->get_component<Collider>();
            if(key_collider->collision(player_collider))
            {
                m_game->get_object("Door")->remove();
                key->remove();
                m_game->get_object("Ball")->get_component<Sprite>()->z_layer = 2;
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
    game.add_system(std::make_shared<DoorSystem>());
    
    game.get_root()->set_name("Root");

    {
        auto background = game.add_object("Background");
        background->transform()->pos = { 0, 0 };
        auto sprite = background->add_component<Sprite>();
        sprite->name.s() = "sprites/tiles/first.png";
        sprite->size = { 800, 600 };
        sprite->z_layer = -1;
    }

    for(int i = 0; i < 2; i++)
    {
        auto box = game.add_object("Player_" + std::to_string(i));

        auto box_transform = box->transform();
        box_transform->pos.x = 50 + 50 * (i % 10);
        box_transform->pos.y = 500 + 10 * (i / 10);

        auto box_collider = box->add_component<TriggerCollider>();
        box_collider->rect = { 50, 50 };

        auto sprite = box->add_component<Sprite>();
        sprite->size = { 50, 50 };
        sprite->name.s() = "sprites/player_" + std::to_string(i % 2 + 1) + ".png";
        sprite->z_layer = 3;
    }

    {
        auto ball = game.add_object("Ball");
        auto ball_transform = ball->transform();
        ball_transform->pos = { 50, 50 };
        
        auto sprite = ball->add_component<Sprite>();
        sprite->size = { 100, 100 };
        sprite->name.s() = "sprites/ball.png";
        sprite->z_layer = 4;
    }

    {
        std::ifstream map_file;
        map_file.open("level_1.map");

        while(true)
        {
            std::string type;
            map_file >> type;
            if(type[0] != '[')
            {
                continue;
            } else if(type == "[END]")
            {
                break;
            } else if(type == "[WALL]" || type == "[DOOR]" || type == "[KEY]")
            {
                int x, y;

                auto wall = game.add_object();

                auto wall_transform = wall->transform();
                
                map_file >> x >> y;
                wall_transform->pos = { x, y };

                std::shared_ptr<Collider> wall_collider;
                if(type == "[KEY]")
                {
                    wall_collider = wall->add_component<TriggerCollider>();
                } else {
                    wall_collider = wall->add_component<Collider>();
                }
                wall_collider->pos = wall_transform->pos;

                map_file >> x >> y;
                wall_collider->rect = { x, y };

                if(type == "[DOOR]")
                {
                    wall->set_name("Door");
                    
                    auto sprite = wall->add_component<Sprite>();
                    sprite->size = wall_collider->rect;
                    sprite->name.s() = "sprites/door.png";
                    sprite->z_layer = 2;
                } else if(type == "[KEY]")
                {
                    wall->set_name("Key");
                    
                    auto sprite = wall->add_component<Sprite>();
                    sprite->size = wall_collider->rect;
                    sprite->name.s() = "sprites/key.png";
                    sprite->z_layer = 2;
                }
            } else {
                warning("Unknown map object type \"" + type + "\"");
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
