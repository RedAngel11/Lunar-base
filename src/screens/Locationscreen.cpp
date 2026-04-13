#include "LocationScreen.h"

LocationScreen::LocationScreen(const sf::Font& f) : font(f) {
    title.setFont(font);
    title.setString(sf::String(L"Выбор локации"));
    title.setCharacterSize(28);
    title.setFillColor(sf::Color::White);
    title.setPosition(40, 70);

    description.setFont(font);
    description.setString(sf::String(L"Доступные зоны:\n• Кратер Шеклтон (низкая радиация)\n• Море Спокойствия (лёгкая посадка)\n• Возвышенности (обзор для связи)"));
    description.setCharacterSize(18);
    description.setFillColor(sf::Color(200, 200, 200));
    description.setPosition(40, 120);
}
void LocationScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {}
void LocationScreen::draw(sf::RenderTarget& target) const {
    target.draw(title);
    target.draw(description);
}