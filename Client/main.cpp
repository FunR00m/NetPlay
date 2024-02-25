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
        status = 0;
        about = false;
    }

    void tick() override
    {
        if(status == 0)
        {
            auto menu = m_game->get_client_root()->get_child("Menu");
            if(m_game->get_controller()->get_state(SDLK_RETURN))
            {
                m_game->get_controller()->set_state(SDLK_RETURN, false);

                if(about)
                {
                    about = false;
                } else if(selection == 0)
                {
                    m_game->connect("localhost:8001");
                    status = 1;
                    hide_menu();
                    return;
                } else if(selection == 1)
                {
                    about = true;
                } else if(selection == 2)
                {
                    m_game->stop();
                }
            }

            auto about_obj = menu->get_child("About");
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

                auto selector = menu->get_child("Selector");
                selector->transform()->pos = {275, 225 + selection * 86 };
            }
        } else if(status == 1)
        {
            if(m_game->get_controller()->get_state(SDLK_ESCAPE))
            {
                m_game->get_controller()->set_state(SDLK_ESCAPE, false);
                show_pause();
                selection = 0;
                status = 2;
            }
        } else if(status == 2)
        {
            auto pause = m_game->get_client_root()->get_child("Pause");
            if(m_game->get_controller()->get_state(SDLK_ESCAPE))
            {
                m_game->get_controller()->set_state(SDLK_ESCAPE, false);
                hide_pause();
                status = 1;
            }
            if(m_game->get_controller()->get_state(SDLK_RETURN))
            {
                m_game->get_controller()->set_state(SDLK_RETURN, false);

                if(selection == 0)
                {
                    status = 1;
                    hide_pause();
                    return;
                } else if(selection == 1)
                {
                    debug("Hello");
                    debug(std::to_string(m_game->get_objects().size()));
                } else if(selection == 2)
                {
                    m_game->disconnect();
                    hide_pause();
                    show_menu();
                    status = 0;
                    selection = 0;
                    return;
                } else if(selection == 3)
                {
                    m_game->stop();
                    return;
                }
            }

            if(m_game->get_controller()->get_state(SDLK_UP))
            {
                m_game->get_controller()->set_state(SDLK_UP, false);
                selection -= 1;
                selection = (selection + 4) % 4;
            }
            if(m_game->get_controller()->get_state(SDLK_DOWN))
            {
                m_game->get_controller()->set_state(SDLK_DOWN, false);
                selection += 1;
                selection = selection % 4;
            }

            auto selector = pause->get_child("Selector");
            selector->transform()->pos = {285, 245 + selection * 80 };
        }
    }

    void show_pause()
    {
        auto pause = m_game->get_client_root()->get_child("Pause");
        for(auto object : pause->get_children())
        {
            object->get_component<Sprite>()->visible = true;
        }
        pause->get_child("Selector")->get_child("Light")->get_component<Sprite>()->visible = true;
    }

    void hide_pause()
    {
        auto pause = m_game->get_client_root()->get_child("Pause");
        for(auto object : pause->get_children())
        {
            object->get_component<Sprite>()->visible = false;
        }
        pause->get_child("Selector")->get_child("Light")->get_component<Sprite>()->visible = false;
    }

    void show_menu()
    {
        auto menu = m_game->get_client_root()->get_child("Menu");
        for(auto object : menu->get_children())
        {
            object->get_component<Sprite>()->visible = true;
        }
    }

    void hide_menu()
    {
        auto menu = m_game->get_client_root()->get_child("Menu");
        for(auto object : menu->get_children())
        {
            object->get_component<Sprite>()->visible = false;
        }
    }

    void stop() override {}

private:
    GameManager *m_game;
    
    int selection;
    bool about;
    int status;
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

    auto menu = game_manager.add_client_object("Menu");
    {
        auto object = menu->add_child("Background");

        object->transform()->pos = { 0, 0 };

        auto sprite = object->add_component<Sprite>();
        sprite->size = { 800, 600 };
        sprite->name.s() = "sprites/Menu 2.png";
        sprite->z_layer = 0;
    }

    {
        auto object = menu->add_child("Selector");

        object->transform()->pos = { 275, 225 };

        auto sprite = object->add_component<Sprite>();
        sprite->size = { 250, 78 };
        sprite->name.s() = "sprites/menu_selector.png";
        sprite->z_layer = 1;
    }

    {
        auto object = menu->add_child("About");

        object->transform()->pos = { 800, 0 };

        auto sprite = object->add_component<Sprite>();
        sprite->size = { 800, 600 };
        sprite->name.s() = "sprites/menu_about.png";
        sprite->z_layer = 2;
    }

    auto pause = game_manager.add_client_object("Pause");
    {
        auto object = pause->add_child("Background");

        object->transform()->pos = { 0, 0 };

        auto sprite = object->add_component<Sprite>();
        sprite->size = { 800, 600 };
        sprite->name.s() = "sprites/Pause.png";
        sprite->z_layer = 101;
        sprite->visible = false;
    }

    {
        auto selector = pause->add_child("Selector");

        auto sprite = selector->add_component<Sprite>();
        sprite->size = { 237, 35 };
        sprite->name.s() = "sprites/pause_selector.png";
        sprite->z_layer = 102;
        sprite->visible = false;

        {
            auto object = selector->add_child("Light");

            object->transform()->pos = { -33, -110 };

            auto sprite = object->add_component<Sprite>();
            sprite->size = { 300, 200 };
            sprite->name.s() = "sprites/void.png";
            sprite->z_layer = 100;
            sprite->visible = false;
        }
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
