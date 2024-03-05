#ifndef RenderSystem_hpp
#define RenderSystem_hpp

#include <unordered_map>
#include <memory>
#include <string>

#include "ISystem.hpp"
#include "Components/Sprite.hpp"
#include "sys/TextureSDL.hpp"
#include "sys/RendererSDL.hpp"

namespace engine
{

class RenderSystem : public ISystem
{
public:
    using ActiveTexture = sys::TextureSDL;
    using ActiveRenderer = sys::RendererSDL;

    void start(GameManager *game_manager) override;

    void tick() override;

    void stop() override;

    void load_texture(std::string name);
    
    /// @brief Устанавливает заголовок окна
    /// @param title Новый заголовок
    void set_window_title(std::string title);

    /// @brief Устанавливает размер окна
    /// @param width Ширина в пикселах
    /// @param height Высота в пикселах
    void set_window_size(int width, int height);

    /// @brief Устанавливает масштаб окна
    /// @param scale Коэффициент масштабирования
    void set_scale(float scale);

    /// @brief Вкючает полноэкранный режим
    void enable_fullscreen();

    /// @brief Выключает полноэкранный режим
    void disable_fullscreen();

private:
    GameManager *m_game;
    std::unordered_map<std::string, std::shared_ptr<ActiveTexture>> m_textures;
    ActiveRenderer m_renderer;

};

}

#endif
