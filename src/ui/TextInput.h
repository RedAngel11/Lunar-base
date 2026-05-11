#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class TextInput {
    sf::RectangleShape bg;
    sf::Text text;
    sf::Font const& font;
    sf::String input;
    bool active = false;
    sf::String placeholder;

public:
    TextInput(const sf::Font& f, const sf::String& ph, sf::Vector2f pos, sf::Vector2f size);
    void handleEvent(const sf::Event& ev);
    void draw(sf::RenderTarget& target) const;
    void activate();
    void deactivate();
    [[nodiscard]] bool isActive() const;
    [[nodiscard]] double getValue() const;
    [[nodiscard]] const sf::String& getString() const;
    void setText(const sf::String& s);
};