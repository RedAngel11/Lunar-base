#pragma once
#include <SFML/Graphics.hpp>

class IScreen {
public:
    virtual ~IScreen() = default;
    virtual void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) = 0;
    virtual void update(sf::Time dt) = 0;
    virtual void draw(sf::RenderTarget& target) const = 0;
};
