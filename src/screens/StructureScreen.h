// src/screens/StructureScreen.hpp
#pragma once
#include "IScreen.h"
#include "../core/SelectionData.h"  // 🔑 Для StructureType
#include "../ui/Button.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>
#include <utility>  // 🔑 Для std::pair

class StructureScreen : public IScreen {
    sf::Font const& font;
    sf::Text title;

    // 🔑 ОБЯЗАТЕЛЬНО: объявляем поля класса
    std::vector<std::pair<std::unique_ptr<Button>, StructureType>> structureButtons;
    std::function<void(StructureType)> onSelectCallback;

public:
    explicit StructureScreen(const sf::Font& f);
    void setOnSelectCallback(std::function<void(StructureType)> cb);

    void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) override;
    void update(sf::Time dt) override {}
    void draw(sf::RenderTarget& target) const override;
};