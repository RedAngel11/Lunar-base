#include "MaterialScreen.h"
#include <memory>

MaterialScreen::MaterialScreen(const sf::Font& f) : font(f) {
    title.setFont(font);
    title.setString(sf::String(L"🧱 Выбор материалов"));
    title.setCharacterSize(28);
    title.setFillColor(sf::Color::White);
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width/2, tb.top + tb.height/2);
    title.setPosition(400, 70);

    std::vector<std::pair<sf::String, MaterialType>> options = {
        {sf::String(L"🪨 Реголитовый бетон"), MaterialType::RegolithConcrete},
        {sf::String(L"❄️ Аэрогель"), MaterialType::Aerogel},
        {sf::String(L"⚙️ Титановый сплав"), MaterialType::TitaniumAlloy},
        {sf::String(L"🫧 Полимерная пена"), MaterialType::PolymerFoam}
    };

    float y = 150;
    for (const auto& [label, type] : options) {
        auto btn = std::make_unique<Button>(font, label, sf::Vector2f(200, y), sf::Vector2f(400, 50));
        materialButtons.push_back({std::move(btn), type, false});
        y += 60;
    }
}

void MaterialScreen::setOnSelectCallback(std::function<void(MaterialType)> cb) {
    onSelectCallback = std::move(cb);
}

void MaterialScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {
    for (auto& opt : materialButtons) {
        opt.button->handleEvent(ev, mousePos);
        if (opt.button->isClicked()) {
            opt.selected = !opt.selected; // Toggle выбор
            opt.button->resetClick();
            if (onSelectCallback) onSelectCallback(opt.type);
        }
    }
}

void MaterialScreen::draw(sf::RenderTarget& target) const {
    target.draw(title);
    for (const auto& opt : materialButtons) {
        opt.button->draw(target);
        // Можно добавить визуальный индикатор выбора, если нужно
    }
}