#pragma once
#include "IScreen.h"
#include <SFML/Graphics.hpp>

class StructureScreen : public IScreen {
    sf::Font const& font;
    sf::Text title, desc;
public:
    explicit StructureScreen(const sf::Font& f);
    void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) override;
    void update(sf::Time) override {}
    void draw(sf::RenderTarget&) const override;
};