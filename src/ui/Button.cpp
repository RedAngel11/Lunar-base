#include "Button.h"

const std::array<sf::Color, 3> Button::colors = {
    sf::Color(60, 60, 70),
    sf::Color(90, 90, 110),
    sf::Color(120, 120, 140)
};

// Принимаем sf::String, передаём его напрямую в sf::Text
Button::Button(const sf::Font& font, const sf::String& label,
               sf::Vector2f pos, sf::Vector2f size)
    : shape(size), text(label, font, 16) {
    shape.setPosition(pos);
    shape.setFillColor(colors[0]);

    // 🔑 ДИНАМИЧЕСКОЕ ЦЕНТРИРОВАНИЕ ТЕКСТА
    sf::FloatRect bounds = text.getLocalBounds();

    // 1. Смещаем "якорь" текста в его геометрический центр
    text.setOrigin(bounds.left + bounds.width / 2.0f,
                   bounds.top + bounds.height / 2.0f);

    // 2. Размещаем якорь ровно в центре кнопки
    text.setPosition(pos.x + size.x / 2.0f,
                     pos.y + size.y / 2.0f);

    text.setFillColor(sf::Color::White);
}

void Button::handleEvent(const sf::Event& ev, const sf::Vector2i& mouse) {
    sf::Vector2f m(mouse.x, mouse.y);
    bool inside = shape.getGlobalBounds().contains(m);

    if (ev.type == sf::Event::MouseMoved) {
        state = inside ? ButtonState::Hover : ButtonState::Normal;
        updateColor();
    } else if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        if (inside) { state = ButtonState::Pressed; updateColor(); clicked = true; }
    } else if (ev.type == sf::Event::MouseButtonReleased) {
        if (inside) { state = ButtonState::Hover; updateColor(); }
        else { state = ButtonState::Normal; updateColor(); }
    }
}

void Button::draw(sf::RenderTarget& target) const {
    target.draw(shape);
    target.draw(text);
}