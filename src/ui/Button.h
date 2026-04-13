#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <array>

enum class ButtonState { Normal, Hover, Pressed };

class Button {
    sf::RectangleShape shape;
    sf::Text text;
    ButtonState state = ButtonState::Normal;
    bool clicked = false;

    static const std::array<sf::Color, 3> colors;

public:
    // Заменили std::string на sf::String
    Button(const sf::Font& font, const sf::String& label,
           sf::Vector2f pos, sf::Vector2f size);
    
    void handleEvent(const sf::Event& ev, const sf::Vector2i& mouse);
    void draw(sf::RenderTarget& target) const;
    
    [[nodiscard]] bool isClicked() const { return clicked; }
    void resetClick() { clicked = false; state = ButtonState::Normal; updateColor(); }
    
private:
    void updateColor() { shape.setFillColor(colors[static_cast<int>(state)]); }
};