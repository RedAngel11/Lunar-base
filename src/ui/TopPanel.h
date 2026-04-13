#pragma once
#include "Button.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

// Общий enum для вкладок. Видим и в TopPanel, и в Game
enum class Tab { Location, Structure, Material, Summary };

class TopPanel {
    std::vector<std::unique_ptr<Button>> buttons;
    Tab activeTab = Tab::Location;
public:
    explicit TopPanel(const sf::Font& font, float windowWidth);
    void handleEvent(const sf::Event& ev, const sf::Vector2i& mouse);
    void draw(sf::RenderTarget& target) const;
    [[nodiscard]] Tab getActiveTab() const { return activeTab; }
};