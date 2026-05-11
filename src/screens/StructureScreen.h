#pragma once
#include "IScreen.h"
#include "../core/SelectionData.h"  // 🔑 Для StructureType
#include "../ui/Button.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>
#include <utility>  // 🔑 Для std::pair
#include "../ui/TextInput.h"

class StructureScreen : public IScreen {
    sf::Font const& font;
    sf::Text title;

    std::vector<std::unique_ptr<Button>> typeButtons;
    std::vector<sf::Text> labels;
    std::vector<std::unique_ptr<TextInput>> inputs;

    StructureType selectedType = StructureType::SealedModule;
    std::function<void(const StructureParams&)> onSelectCallback;

public:
    explicit StructureScreen(const sf::Font& f);
    void setOnSelectCallback(std::function<void(const StructureParams&)> cb);
    void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) override;
    void update(sf::Time dt) override {}
    void draw(sf::RenderTarget& target) const override;
};