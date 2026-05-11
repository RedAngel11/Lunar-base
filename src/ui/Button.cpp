#include "Button.h"

Button::Button(const sf::Font& font, const sf::String& label,
               sf::Vector2f pos, sf::Vector2f size)
    : shape(size), text(label, font, 16) {

    colors = {sf::Color(60, 60, 70), sf::Color(90, 90, 110), sf::Color(120, 120, 140)};

    shape.setPosition(pos);
    shape.setFillColor(colors[0]);
    text.setFillColor(sf::Color::White);

    // 🔑 АВТОМАТИЧЕСКОЕ ЦЕНТРИРОВАНИЕ
    // 1. Получаем реальные границы текста
    sf::FloatRect textBounds = text.getLocalBounds();

    // 2. Смещаем "якорь" (origin) в геометрический центр текста
    text.setOrigin(textBounds.left + textBounds.width / 2.0f,
                   textBounds.top + textBounds.height / 2.0f);

    // 3. Ставим якорь ровно в центр кнопки
    text.setPosition(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f);
}

void Button::setColors(sf::Color normal, sf::Color hover, sf::Color pressed) {
    colors = {normal, hover, pressed};
    updateColor();
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