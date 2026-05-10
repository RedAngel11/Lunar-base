#pragma once
#include <SFML/Graphics.hpp>
#include "../core/SelectionData.h"

class Hotspot {
    sf::CircleShape shape;
    std::string name;
    LocationInfo info;
    bool isHovered = false;
    sf::Text tooltip;
    sf::Font const& font;
    sf::Vector2u mapSize;

public:
    Hotspot(const sf::Font& f, const LocationInfo& data, float radius, sf::Vector2u mapSize);

    // Принимает координаты мыши в пространстве текстуры
    void handleEvent(const sf::Event& ev, const sf::Vector2f& mouseInLocalSpace);
    // Принимает трансформ для отрисовки
    void draw(sf::RenderTarget& target, const sf::Transform& transform) const;

    [[nodiscard]] bool isClicked() const;
    [[nodiscard]] const LocationInfo& getInfo() const { return info; }
    void resetClick();
};