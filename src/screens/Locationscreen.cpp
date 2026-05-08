// src/screens/LocationScreen.cpp
#include "LocationScreen.h"
#include <iostream>

LocationScreen::LocationScreen(const sf::Font& f, const std::string& mapPath) : font(f) {
    if (!moonTexture.loadFromFile(mapPath)) {
        std::cerr << "⚠️ Warning: не удалось загрузить карту: " << mapPath << "\n";
        moonMap.setTextureRect(sf::IntRect(0, 0, 800, 600));
        moonMap.setColor(sf::Color(50, 50, 60));
    } else {
        moonMap.setTexture(moonTexture);
        moonMap.setScale(800.0f / moonTexture.getSize().x, 600.0f / moonTexture.getSize().y);
    }

    // Данные локаций
    std::vector<LocationInfo> locations = {
        { "Море Спокойствия", 1.2, -170, 120, 0.85, 250, 200 },
        { "Кратер Шеклтон", 0.8, -230, 40, 0.92, 550, 450 },
        { "Возвышенности", 1.5, -180, 130, 0.70, 400, 150 }
    };

    for (const auto& loc : locations) {
        hotspots.push_back(std::make_unique<Hotspot>(font, loc, 25.0f));
    }

    infoPanel.setFont(font);
    infoPanel.setCharacterSize(14);
    infoPanel.setFillColor(sf::Color(220, 220, 220));
    infoPanel.setPosition(20, 500);
}

void LocationScreen::setOnSelectCallback(std::function<void(const LocationInfo&)> cb) {
    onSelectCallback = std::move(cb);
}

void LocationScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {
    for (auto& hs : hotspots) {
        hs->handleEvent(ev, mousePos);
        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            if (hs->isClicked()) {
                std::cout << "🎯 Выбрана локация: " << hs->getInfo().name << "\n";
                if (onSelectCallback) onSelectCallback(hs->getInfo());
                hs->resetClick();
            }
        }
    }
}

void LocationScreen::draw(sf::RenderTarget& target) const {
    target.draw(moonMap);
    for (const auto& hs : hotspots) hs->draw(target);

    // 🔑 mutable позволяет менять infoPanel даже в const-методе
    infoPanel.setString(sf::String(L"💡 Наведите курсор на точку для названия, кликните для выбора"));
    target.draw(infoPanel);
}