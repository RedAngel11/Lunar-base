// src/screens/LocationScreen.hpp
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

    // 🔑 mutable позволяет менять это поле даже в const-методе draw()
    mutable sf::Text infoPanel;

    std::function<void(const LocationInfo&)> onSelectCallback;

public:
    LocationScreen(const sf::Font& f, const std::string& mapPath);
    void setOnSelectCallback(std::function<void(const LocationInfo&)> cb);

    void handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) override;
    void update(sf::Time dt) override {}
    void draw(sf::RenderTarget& target) const override;
};