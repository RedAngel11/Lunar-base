#pragma once
#include "IScreen.h"
#include <SFML/Graphics.hpp>
#include <string>

class LocationScreen : public IScreen {
    sf::Font const& font;
    sf::Text title;
    sf::Text description;
public:
    explicit LocationScreen(const sf::Font& f);
    void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) override;
    void update(sf::Time dt) override {}
    void draw(sf::RenderTarget& target) const override;
};