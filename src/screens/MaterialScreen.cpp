#include "MaterialScreen.h"
#include "../ui/TextInput.h"

MaterialScreen::MaterialScreen(const sf::Font& f) : font(f) {
    title.setFont(font);
    title.setString(sf::String(L"Select Construction Material"));
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::White);
    title.setPosition(30, 30);

    std::vector<std::pair<sf::String, MaterialType>> options = {
        {sf::String(L"Regolith Concrete"), MaterialType::RegolithConcrete},
        {sf::String(L"Aerogel"), MaterialType::Aerogel},
        {sf::String(L"Titanium Alloy"), MaterialType::TitaniumAlloy},
        {sf::String(L"Polymer Foam"), MaterialType::PolymerFoam}
    };

    float y = 80;
    for (const auto& [label, type] : options) {
        materialButtons.push_back({std::make_unique<Button>(font, label, sf::Vector2f(100, y), sf::Vector2f(300, 40)), type});
        y += 50;
    }

    customBtn = std::make_unique<Button>(font, sf::String(L"Custom Material"), sf::Vector2f(100, y), sf::Vector2f(300, 40));
    y += 50;

    customInputs.push_back(std::make_unique<TextInput>(font, L"Density (kg/m3)", sf::Vector2f(100, y), sf::Vector2f(140, 25)));
    customInputs.push_back(std::make_unique<TextInput>(font, L"Conductivity", sf::Vector2f(250, y), sf::Vector2f(140, 25)));
    y += 35;
    customInputs.push_back(std::make_unique<TextInput>(font, L"Shielding Factor", sf::Vector2f(100, y), sf::Vector2f(140, 25)));
    customInputs.push_back(std::make_unique<TextInput>(font, L"Durability (years)", sf::Vector2f(250, y), sf::Vector2f(140, 25)));
    y += 35;
    customInputs.push_back(std::make_unique<TextInput>(font, L"In-Situ Factor", sf::Vector2f(100, y), sf::Vector2f(140, 25)));
    customInputs.push_back(std::make_unique<TextInput>(font, L"Dev Time (years)", sf::Vector2f(250, y), sf::Vector2f(140, 25)));

    for (auto& inp : customInputs) inp->setText(L"1.0");
}

void MaterialScreen::setOnSelectCallback(std::function<void(MaterialType)> cb) {
    onSelectCallback = std::move(cb);
}

const CustomMaterialParams& MaterialScreen::getCustomParams() const {
    static CustomMaterialParams p;
    if (isCustomSelected) {
        p.density = customInputs[0]->getValue();
        p.thermalConductivity = customInputs[1]->getValue();
        p.radiationShielding = customInputs[2]->getValue();
        p.durability = customInputs[3]->getValue();
        p.inSituProducibility = customInputs[4]->getValue();
        p.devTimeYears = customInputs[5]->getValue();
        p.isActive = true;
    } else {
        p.isActive = false;
    }
    return p;
}

void MaterialScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {
    for (auto& opt : materialButtons) {
        opt.button->handleEvent(ev, mousePos);
        if (opt.button->isClicked()) {
            isCustomSelected = false;
            opt.button->resetClick();
            if (onSelectCallback) onSelectCallback(opt.type);
        }
    }

    if (customBtn) {
        customBtn->handleEvent(ev, mousePos);
        if (customBtn->isClicked()) {
            isCustomSelected = true;
            customBtn->resetClick();
            if (onSelectCallback) onSelectCallback(MaterialType::None);
        }
    }

    if (isCustomSelected) {
        for (auto& inp : customInputs) inp->handleEvent(ev);
    } else {
        for (auto& inp : customInputs) inp->deactivate();
    }
}

void MaterialScreen::draw(sf::RenderTarget& target) const {
    target.draw(title);
    for (const auto& opt : materialButtons) opt.button->draw(target);
    if (customBtn) customBtn->draw(target);
    if (isCustomSelected) {
        for (const auto& inp : customInputs) inp->draw(target);
    }
}