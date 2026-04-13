#pragma once
#include "IScreen.h"
#include <SFML/Graphics.hpp>

class MaterialScreen : public IScreen {
    sf::Font const& font;
    sf::Text title, desc;
public:
    explicit MaterialScreen(const sf::Font& f);
    void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) override {}
    void update(sf::Time dt) override {}
    void draw(sf::RenderTarget& target) const override;
};