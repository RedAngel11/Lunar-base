// src/ui/Hotspot.cpp
#include "Hotspot.h"

Hotspot::Hotspot(const sf::Font& f, const LocationInfo& data, float radius)
    : font(f), info(data), name(data.name) {  // Копируем name отдельно для удобства

    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(static_cast<float>(info.hotspotX), static_cast<float>(info.hotspotY));
    shape.setFillColor(sf::Color(100, 150, 255, 180));
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);

    tooltip.setFont(font);
    tooltip.setString(sf::String(L"📍 ") + sf::String(name));
    tooltip.setCharacterSize(14);
    tooltip.setFillColor(sf::Color::White);
    tooltip.setOutlineColor(sf::Color::Black);
    tooltip.setOutlineThickness(1);
}

void Hotspot::handleEvent(const sf::Event& ev, const sf::Vector2i& mouse) {
    sf::Vector2f m(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
    bool inside = shape.getGlobalBounds().contains(m);

    if (ev.type == sf::Event::MouseMoved) {
        isHovered = inside;
        shape.setFillColor(isHovered ? sf::Color(150, 200, 255, 220) : sf::Color(100, 150, 255, 180));

        if (isHovered) {
            sf::FloatRect bounds = tooltip.getLocalBounds();
            tooltip.setOrigin(bounds.left + bounds.width/2, bounds.top + bounds.height/2);
            tooltip.setPosition(static_cast<float>(info.hotspotX), static_cast<float>(info.hotspotY - 30));
        }
    }
}

void Hotspot::draw(sf::RenderTarget& target) const {
    target.draw(shape);
    if (isHovered) target.draw(tooltip);
}

bool Hotspot::isClicked() const { return isHovered; }
void Hotspot::resetClick() { isHovered = false; shape.setFillColor(sf::Color(100, 150, 255, 180)); }