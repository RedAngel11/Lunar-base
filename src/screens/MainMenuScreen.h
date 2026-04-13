#pragma once
#include "IScreen.h"
#include "../ui/Button.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <functional>

class MainMenuScreen : public IScreen {
    sf::Sprite background;
    sf::Texture bgTexture;
    sf::Text title;
    std::unique_ptr<Button> startButton;
    std::function<void()> onStartCallback;

public:
    MainMenuScreen(const sf::Font& font, const std::string& bgPath);
    void setOnStartCallback(std::function<void()> callback);

    // 🔑 ИСПРАВЛЕНО: сигнатура теперь точно совпадает с IScreen
    void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) override;
    void update(sf::Time dt) override {}
    void draw(sf::RenderTarget& target) const override;
};