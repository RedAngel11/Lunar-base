#include "TextInput.h"
#include <string>

TextInput::TextInput(const sf::Font& f, const sf::String& ph, sf::Vector2f pos, sf::Vector2f size)
    : font(f), placeholder(ph), input("") {
    bg.setSize(size);
    bg.setPosition(pos);
    bg.setFillColor(sf::Color(40, 40, 50));
    bg.setOutlineThickness(1);
    bg.setOutlineColor(sf::Color(100, 100, 100)); // Цвет по умолчанию

    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setPosition(pos.x + 5, pos.y + (size.y / 2.f) - 8);
}

void TextInput::handleEvent(const sf::Event& ev) {
    if (ev.type == sf::Event::MouseButtonPressed) {
        if (bg.getGlobalBounds().contains(ev.mouseButton.x, ev.mouseButton.y)) {
            activate();
        } else {
            deactivate();
        }
    }

    if (active) {
        if (ev.type == sf::Event::TextEntered) {
            if (ev.text.unicode < 128) {
                input += static_cast<char>(ev.text.unicode);
            }
        }
        if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::BackSpace) {
            if (!input.isEmpty()) {
                input.erase(input.getSize() - 1);
            }
        }
    }
    text.setString(input.isEmpty() ? placeholder : input);
}

void TextInput::draw(sf::RenderTarget& target) const {
    // 🔑 ИСПРАВЛЕНИЕ: Создаем временную копию, чтобы изменить цвет только для отрисовки
    sf::RectangleShape tempBg = bg;
    tempBg.setOutlineColor(active ? sf::Color::White : sf::Color(100, 100, 100));

    target.draw(tempBg);
    target.draw(text);
}

void TextInput::activate() {
    active = true;
    input = L"";
}
void TextInput::deactivate() {
    active = false;
}
bool TextInput::isActive() const { return active; }

double TextInput::getValue() const {
    try {
        return std::stod(input.toAnsiString());
    } catch (...) {
        return 0.0;
    }
}
const sf::String& TextInput::getString() const { return input; }
void TextInput::setText(const sf::String& s) { input = s; text.setString(s); }