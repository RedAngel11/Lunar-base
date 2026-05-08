// src/ui/Hotspot.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../core/SelectionData.h"  // 🔑 КЛЮЧЕВОЕ: подключаем LocationInfo

class Hotspot {
    sf::CircleShape shape;
    std::string name;
    LocationInfo info;  // Теперь компилятор знает этот тип
    bool isHovered = false;
    sf::Text tooltip;
    sf::Font const& font;

public:
    Hotspot(const sf::Font& f, const LocationInfo& data, float radius);
    
    void handleEvent(const sf::Event& ev, const sf::Vector2i& mouse);
    void draw(sf::RenderTarget& target) const;
    
    [[nodiscard]] bool isClicked() const;
    [[nodiscard]] const LocationInfo& getInfo() const { return info; }
    void resetClick();
};