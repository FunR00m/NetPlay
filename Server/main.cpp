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
#include "Components/Transform.hpp"
#include "Components/Sprite.hpp"
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
		auto obj = m_game->get_root()->get_child("Box");
		std::shared_ptr<Transform> transform = obj->get_component<Transform>();
		if(transform->pos.x >= 200)
		{
			transform->motion.x = -5;
		} else if(transform->pos.x <= 90)
		{
			transform->motion.x = 5;
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

    auto box = game.add_object("Box");

    auto box_transform = box->add_component<Transform>();
    box_transform->move_event += move_listener; 
    box_transform->pos.x = 90;
    box_transform->pos.y = 90;
    box_transform->motion.x = 5;

    auto sprite = box->add_component<Sprite>();
    sprite->pos = box_transform->pos;
    sprite->size.x = 200;
    sprite->size.y = 200;
    sprite->name.s() = "image.png";
    
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
