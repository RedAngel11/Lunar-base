#pragma once
#include "IScreen.h"
#include "../ui/Hotspot.h"
#include "../core/SelectionData.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>

class LocationScreen : public IScreen {
    sf::Sprite moonMap;
    sf::Texture moonTexture;
    std::vector<std::unique_ptr<Hotspot>> hotspots;
    sf::Font const& font;
    mutable sf::Text infoPanel;
    std::function<void(const LocationInfo&)> onSelectCallback;

    // Состояние камеры
    sf::Vector2f mapOffset;
    sf::Vector2f dragStartMouse;
    sf::Vector2f dragStartOffset;
    bool isDragging = false;
    float mapScale = 0.45f; // Начальный масштаб (3200px -> ~1440px, удобно для зума)

public:
    LocationScreen(const sf::Font& f, const std::string& mapPath);
    void setOnSelectCallback(std::function<void(const LocationInfo&)> cb);
    void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) override;
    void update(sf::Time dt) override {}
    void draw(sf::RenderTarget& target) const override;
};