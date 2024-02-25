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
#include "Components/NullComp.hpp"

#include "Systems/RenderSystem.hpp"

using namespace engine;


class MenuSystem : public ISystem
{
public:
    void start(GameManager *game_manager) override
    {
        m_game = game_manager;
        selection = 0;
        about = false;
    }

    void tick() override
    {
        if(!m_game->is_connected())
        {
            if(m_game->get_controller()->get_state(SDLK_SPACE))
            {
                m_game->get_controller()->set_state(SDLK_SPACE, false);

                if(about)
                {
                    about = false;
                } else if(selection == 0)
                {
                    m_game->connect("localhost:8001");
                } else if(selection == 1)
                {
                    about = true;
                } else if(selection == 2)
                {
                    m_game->stop();
                }
            }

            auto about_obj = m_game->get_root()->get_child("About");
            if(about)
            {
                about_obj->get_component<Transform>()->pos = { 0, 0 };
            } else {
                about_obj->get_component<Transform>()->pos = { 800, 0 };

                if(m_game->get_controller()->get_state(SDLK_UP))
                {
                    m_game->get_controller()->set_state(SDLK_UP, false);
                    selection -= 1;
                    selection = (selection + 3) % 3;
                }
                if(m_game->get_controller()->get_state(SDLK_DOWN))
                {
                    m_game->get_controller()->set_state(SDLK_DOWN, false);
                    selection += 1;
                    selection = selection % 3;
                }

                auto selector = m_game->get_root()->get_child("Selector");
                auto transform = selector->get_component<Transform>();
                transform->pos = {275, 225 + selection * 86 };
            }
        }
    }

    void stop() override {}

private:
    GameManager *m_game;
    
    int selection;
    bool about;
};

int game_test()
{
    GameManager game_manager;

    game_manager.register_component<Transform>("Transform");
    game_manager.register_component<Sprite>("Sprite");
    game_manager.register_component<NullComp>("Collider");
    // game_manager.register_component<NullComp>("TriggerCollider");

    game_manager.add_system(std::make_shared<MenuSystem>());
    game_manager.add_system(std::make_shared<RenderSystem>());

    {
        auto menu = game_manager.add_object("Menu");

        auto transform = menu->add_component<Transform>();
        transform->pos = { 0, 0 };

        auto sprite = menu->add_component<Sprite>();
        sprite->size = { 800, 600 };
        sprite->name.s() = "sprites/Menu 2.png";
    }

    {
        auto selector = game_manager.add_object("Selector");

        auto transform = selector->add_component<Transform>();
        transform->pos = { 275, 225 };

        auto sprite = selector->add_component<Sprite>();
        sprite->size = { 250, 78 };
        sprite->name.s() = "sprites/menu_selector.png";
    }

    {
        auto selector = game_manager.add_object("About");

        auto transform = selector->add_component<Transform>();
        transform->pos = { 800, 0 };

        auto sprite = selector->add_component<Sprite>();
        sprite->size = { 800, 600 };
        sprite->name.s() = "sprites/menu_about.png";
    }

    game_manager.start();

    return 0;
}

int render_test()
{
    sys::RendererSDL renderer;
    renderer.setup();

    auto texture = std::make_shared<sys::TextureSDL>();
    texture->load("image.png");

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
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        std::cout << "YES" << std::endl;
    }

    std::cout << "NO" << std::endl;

    return 0;
}

int main(int argc, const char * argv[]) {
    // GameManager game_manager;
    // auto box = game_manager.add_object();
    // box->set_name("Box");
    
    // std::cout << game_manager.get_root()->get_child("Box")->get_name() << std::endl;

    return game_test();
}
