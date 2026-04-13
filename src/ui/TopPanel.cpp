#include "TopPanel.h"
#include <array>

TopPanel::TopPanel(const sf::Font& font, float windowWidth) {
    const std::array<sf::String, 4> tabNames = {
        sf::String(L"Локация"),
        sf::String(L"Структура"),
        sf::String(L"Материалы"),
        sf::String(L"Итог")
    };
    float btnW = windowWidth / 4.0f;

    for (size_t i = 0; i < tabNames.size(); ++i) {
        buttons.push_back(std::make_unique<Button>(
            font, tabNames[i], sf::Vector2f(i * btnW, 0), sf::Vector2f(btnW, 40)
        ));
    }
}

void TopPanel::handleEvent(const sf::Event& ev, const sf::Vector2i& mouse) {
    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i]->handleEvent(ev, mouse);
        if (buttons[i]->isClicked()) {
            activeTab = static_cast<Tab>(i);
            buttons[i]->resetClick();
        } else {
            buttons[i]->resetClick();
        }
    }
}

void TopPanel::draw(sf::RenderTarget& target) const {
    for (const auto& btn : buttons) btn->draw(target);
}