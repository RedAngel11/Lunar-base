#pragma once
#include "IScreen.h"
#include "../core/SelectionData.h"
#include "../ui/Button.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>

class MaterialScreen : public IScreen {
    sf::Font const& font;
    sf::Text title;

    struct MaterialOption {
        std::unique_ptr<Button> button;
        MaterialType type;
        bool selected = false;
    };
    std::vector<MaterialOption> materialButtons;

    std::function<void(MaterialType)> onSelectCallback;

public:
    explicit MaterialScreen(const sf::Font& f);
    void setOnSelectCallback(std::function<void(MaterialType)> cb);

    void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) override;
    void update(sf::Time dt) override {}
    void draw(sf::RenderTarget& target) const override;
};