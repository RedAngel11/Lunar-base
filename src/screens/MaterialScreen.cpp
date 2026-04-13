#include "MaterialScreen.h"

MaterialScreen::MaterialScreen(const sf::Font& f) : font(f) {
    title.setFont(font); 
    title.setString(sf::String(L"Выбор материалов"));
    title.setCharacterSize(28); 
    title.setFillColor(sf::Color::White);
    title.setPosition(40, 70);

    desc.setFont(font); 
    desc.setString(sf::String(
        L"Доступные материалы:\n"
        L"• Реголитовый бетон (дешёвый, средний срок службы)\n"
        L"• Аэрогель (лёгкий, высокая изоляция)\n"
        L"• Титановый сплав (прочный, дорогой)\n"
        L"• Полимерная пена (гибкая, для куполов)"
    ));
    desc.setCharacterSize(18); 
    desc.setFillColor(sf::Color(200, 200, 200));
    desc.setPosition(40, 120);
}

void MaterialScreen::draw(sf::RenderTarget& target) const {
    target.draw(title); 
    target.draw(desc);
}