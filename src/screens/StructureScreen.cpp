// src/screens/StructureScreen.cpp
#include "StructureScreen.h"
#include <memory>  // 🔑 Для std::make_unique

StructureScreen::StructureScreen(const sf::Font& f) : font(f) {
    title.setFont(font);
    title.setString(sf::String(L"🏗️ Тип структуры"));
    title.setCharacterSize(28);
    title.setFillColor(sf::Color::White);

    // Центрирование
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width/2, tb.top + tb.height/2);
    title.setPosition(400, 70);

    // Создаём кнопки
    std::vector<std::pair<sf::String, StructureType>> options = {
        {sf::String(L"🔒 Герметичный модуль"), StructureType::SealedModule},
        {sf::String(L"🕳️ Подземный бункер"), StructureType::UndergroundBunker},
        {sf::String(L"🎈 Надувной купол"), StructureType::InflatableDome},
        {sf::String(L"🖨️ 3D-печать из реголита"), StructureType::RegolithPrinted}
    };

    float y = 150;
    for (const auto& [label, type] : options) {
        auto btn = std::make_unique<Button>(font, label, sf::Vector2f(200, y), sf::Vector2f(400, 50));
        structureButtons.push_back({std::move(btn), type});
        y += 60;
    }
}

void StructureScreen::setOnSelectCallback(std::function<void(StructureType)> cb) {
    onSelectCallback = std::move(cb);
}

void StructureScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {
    for (auto& [btn, type] : structureButtons) {
        btn->handleEvent(ev, mousePos);
        if (btn->isClicked()) {
            for (auto& [b, t] : structureButtons) b->resetClick();
            btn->resetClick();
            if (onSelectCallback) onSelectCallback(type);
        }
    }
}

void StructureScreen::draw(sf::RenderTarget& target) const {
    target.draw(title);
    for (const auto& [btn, type] : structureButtons) {
        btn->draw(target);
    }
}