#include "StructureScreen.h"
#include "../ui/TextInput.h"
#include <memory>  // 🔑 Для std::make_unique
#include <iostream>

StructureScreen::StructureScreen(const sf::Font& f) : font(f) {
    title.setFont(font);
    title.setString(sf::String(L"Base Structure Configuration"));
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::White);
    title.setPosition(30, 20);

    std::vector<sf::String> typeNames = {
        sf::String(L"Sealed Module"),
        sf::String(L"Underground Bunker"),
        sf::String(L"Inflatable Dome"),
        sf::String(L"3D-Printed Regolith")
    };
    std::vector<StructureType> typeEnums = {
        StructureType::SealedModule,
        StructureType::UndergroundBunker,
        StructureType::InflatableDome,
        StructureType::RegolithPrinted
    };

    float y = 70;
    for (size_t i = 0; i < typeNames.size(); ++i) {
        auto btn = std::make_unique<Button>(font, typeNames[i], sf::Vector2f(100, y), sf::Vector2f(250, 40));
        // Сохраняем связь кнопки и типа через индекс (просто для передачи в callback)
        typeButtons.push_back(std::move(btn));
        y += 50;
    }
    selectedType = StructureType::SealedModule;

    std::vector<sf::String> labelTexts = {
        sf::String(L"Wall Thickness (m)"),
        sf::String(L"Total Volume (m3)"),
        sf::String(L"Number of Rooms"),
        sf::String(L"Buried Fraction (0-1)")
    };

    y = 290;
    for (size_t i = 0; i < labelTexts.size(); ++i) {
        sf::Text lbl;
        lbl.setFont(font);
        lbl.setString(labelTexts[i]);
        lbl.setCharacterSize(16);
        lbl.setFillColor(sf::Color(200, 200, 200));
        lbl.setPosition(50, y);
        labels.push_back(lbl);

        inputs.push_back(std::make_unique<TextInput>(font, L"", sf::Vector2f(260, y - 5), sf::Vector2f(180, 28)));
        y += 40;
    }

    inputs[0]->setText(L"0.5");
    inputs[1]->setText(L"150");
    inputs[2]->setText(L"3");
    inputs[3]->setText(L"0.3");
}

void StructureScreen::setOnSelectCallback(std::function<void(const StructureParams&)> cb) {
    onSelectCallback = std::move(cb);
}

void StructureScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {
    // 🔑 ОБРАБОТКА КЛИКОВ ПО ТИПАМ
    for (size_t i = 0; i < typeButtons.size(); ++i) {
        typeButtons[i]->handleEvent(ev, mousePos);
        if (typeButtons[i]->isClicked()) {
            selectedType = static_cast<StructureType>(i);
            typeButtons[i]->resetClick();

            // 🔑 Сразу отправляем данные при выборе типа
            StructureParams p;
            p.type = selectedType;
            p.wallThickness = inputs[0]->getValue();
            p.compartmentVolumes = {inputs[1]->getValue()};
            p.numCompartments = static_cast<int>(inputs[2]->getValue());
            p.buriedFraction = std::clamp(inputs[3]->getValue(), 0.0, 1.0);

            if (onSelectCallback) onSelectCallback(p);
        }
    }

    // Обработка полей ввода
    for (auto& inp : inputs) inp->handleEvent(ev);

    // Сохраняем при Enter
    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter) {
        StructureParams p;
        p.type = selectedType;
        p.wallThickness = inputs[0]->getValue();
        p.compartmentVolumes = {inputs[1]->getValue()};
        p.numCompartments = static_cast<int>(inputs[2]->getValue());
        p.buriedFraction = std::clamp(inputs[3]->getValue(), 0.0, 1.0);

        if (onSelectCallback) onSelectCallback(p);
    }
}

void StructureScreen::draw(sf::RenderTarget& target) const {
    target.draw(title);
    for (const auto& btn : typeButtons) btn->draw(target);
    for (const auto& lbl : labels) target.draw(lbl);
    for (const auto& inp : inputs) inp->draw(target);
}