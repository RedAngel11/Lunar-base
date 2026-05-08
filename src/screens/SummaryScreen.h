#pragma once
#include "IScreen.h"
#include "../core/SelectionData.h"
#include "../ui/Button.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <string>

class SummaryScreen : public IScreen {
    sf::Font const& font;
    SelectionData const& data;

    // 🔑 mutable разрешает менять текст внутри const-метода draw()
    mutable sf::Text headerText, locationInfo, structureInfo, materialsInfo;
    std::unique_ptr<Button> calculateButton;
    std::function<void()> onCalculateCallback;

public:
    SummaryScreen(const sf::Font& f, SelectionData& sharedData);
    void setOnCalculateCallback(std::function<void()> cb);
    
    void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) override;
    void update(sf::Time dt) override {}
    void draw(sf::RenderTarget& target) const override;
};