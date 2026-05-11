#include "Hotspot.h"

Hotspot::Hotspot(const sf::Font& f, const LocationInfo& data, float radius, sf::Vector2u mSize)
    : font(f), info(data), name(data.name), mapSize(mSize) {

    float pixelX = info.relX * static_cast<float>(mapSize.x);
    float pixelY = info.relY * static_cast<float>(mapSize.y);

    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setPosition(pixelX, pixelY); // Жёстко привязано к текстуре

    shape.setFillColor(sf::Color(20, 40, 100, 200));

    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::White);

    tooltip.setFont(font);
    tooltip.setString(sf::String(L" ") + sf::String(name));
    tooltip.setCharacterSize(14);
    tooltip.setFillColor(sf::Color::White);
    tooltip.setOutlineColor(sf::Color::Black);
    tooltip.setOutlineThickness(1);
}

void Hotspot::handleEvent(const sf::Event& ev, const sf::Vector2f& mouseInLocalSpace) {
    bool inside = shape.getGlobalBounds().contains(mouseInLocalSpace);

    if (ev.type == sf::Event::MouseMoved) {
        isHovered = inside;
        shape.setFillColor(isHovered ? sf::Color(80, 160, 255, 230) : sf::Color(20, 40, 100, 200));

        if (isHovered) {
            sf::FloatRect bounds = tooltip.getLocalBounds();
            tooltip.setOrigin(bounds.left + bounds.width/2, bounds.top + bounds.height/2);
            // Тултип позиционируется в локальных координатах (трансформ применится при draw)
            tooltip.setPosition(shape.getPosition().x, shape.getPosition().y - 30.0f);
        }
    }
}

void Hotspot::draw(sf::RenderTarget& target, const sf::Transform& transform) const {
    target.draw(shape, transform);
    if (isHovered) target.draw(tooltip, transform);
}

bool Hotspot::isClicked() const { return isHovered; }
void Hotspot::resetClick() { isHovered = false; shape.setFillColor(sf::Color(20, 40, 100, 200)); }