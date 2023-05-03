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

#include "Components/Transform.hpp"
#include "Components/Sprite.hpp"
#include "Systems/RenderSystem.hpp"
#include "sys/InetNetworker.hpp"

using namespace engine;


class TestSystem : public ISystem
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
                debug("Transform!");
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
    game_manager.register_component<Transform>("Transform");
    game_manager.register_component<Sprite>("Sprite");
    game_manager.add_system(std::make_shared<TestSystem>());
    game_manager.add_system(std::make_shared<RenderSystem>());
    game_manager.start();

    return 0;
}

int render_test()
{
    sys::RendererSDL renderer;
    renderer.setup();

    auto texture = std::make_shared<sys::TextureSDL>();
    texture->load("/home/fedor/Documents/Projects/netplay/image.png");

    auto sprite = std::make_shared<Sprite>();
    sprite->pos = { 50, 50 };
    sprite->size = { 200, 200 };
    sprite->name.s() = "hello";
    sprite->texture = texture;
    
    bool running = true;
    int n = 2;
    while(running)
    {
        n += 2;
        if(n % 2 != 0)
        {
            running = false;
        }
        
        renderer.render_sprite(sprite);
        renderer.refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}

int main(int argc, const char * argv[]) {
    // GameManager game_manager;
    // auto box = game_manager.add_object();
    // box->set_name("Box");
    
    // std::cout << game_manager.get_root()->get_child("Box")->get_name() << std::endl;

    return game_test();
}
