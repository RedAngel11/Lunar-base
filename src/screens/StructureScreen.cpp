#include "StructureScreen.h"

StructureScreen::StructureScreen(const sf::Font& f) : font(f) {
    title.setFont(font); title.setString(sf::String(L"Тип структуры"));
    title.setCharacterSize(28); title.setFillColor(sf::Color::White);
    title.setPosition(40, 70);

    desc.setFont(font); desc.setString(sf::String(L"Варианты:\n• Герметичный модуль\n• Подземный бункер\n• Надувной купол\n• 3D-печать из реголита"));
    desc.setCharacterSize(18); desc.setFillColor(sf::Color(200, 200, 200));
    desc.setPosition(40, 120);
}
void StructureScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {
    // Пусто, но сигнатура должна совпадать
}
void StructureScreen::draw(sf::RenderTarget& target) const {
    target.draw(title); target.draw(desc);
}