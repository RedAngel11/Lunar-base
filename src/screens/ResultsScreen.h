#pragma once
#include "IScreen.h"
#include "../core/CalculationReport.h"
#include "../ui/Button.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <functional>

class ResultsScreen : public IScreen {
    sf::Font const& font;
    CalculationReport const& report;
    std::unique_ptr<Button> backButton;
    std::vector<sf::Text> textLines;

    sf::Sprite resultImage;
    sf::Texture resultTexture;

    std::function<void()> onBackCallback;

    void buildTextLines();

public:
    ResultsScreen(const sf::Font& f, const CalculationReport& rep);
    void setOnBackCallback(std::function<void()> cb);
    void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) override;
    void update(sf::Time dt) override {}
    void draw(sf::RenderTarget& target) const override;
};