#include "StructureScreen.h"
#include "../ui/TextInput.h"
#include <memory>  // 🔑 Для std::make_unique
#include <iostream>

StructureScreen::StructureScreen(const sf::Font& f) : font(f) {
    title.setFont(font);
    title.setString(sf::String(L"Конфигурация базы"));
    title.setCharacterSize(30); // 🔑 Увеличили шрифт
    title.setFillColor(sf::Color::White);
    // 🔑 Центрируем заголовок
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
    title.setPosition(400.0f, 70.0f); // 🔑 Сдвинули ниже панели

    // Кнопки типов структуры (по центру)
    std::vector<sf::String> typeNames = {
        sf::String(L"Герметичный модуль"),
        sf::String(L"Подземный бункер"),
        sf::String(L"Надувной купол"),
        sf::String(L"3D-печать из реголита")
    };
    std::vector<StructureType> typeEnums = {
        StructureType::SealedModule,
        StructureType::UndergroundBunker,
        StructureType::InflatableDome,
        StructureType::RegolithPrinted
    };

    float y = 130;
    for (size_t i = 0; i < typeNames.size(); ++i) {
        // 🔑 Центрируем кнопки: (800 - 300) / 2 = 250
        auto btn = std::make_unique<Button>(font, typeNames[i], sf::Vector2f(250, y), sf::Vector2f(300, 40));
        typeButtons.push_back(std::move(btn));
        y += 50;
    }
    selectedType = StructureType::SealedModule;

    // Подписи и поля ввода
    std::vector<sf::String> labelTexts = {
        sf::String(L"Толщина стенки (м)"),
        sf::String(L"Общий объём (м3)"),
        sf::String(L"Кол-во помещений"),
        sf::String(L"Доля заглубления")
    };

    y = 350;
    for (size_t i = 0; i < labelTexts.size(); ++i) {
        sf::Text lbl;
        lbl.setFont(font);
        lbl.setString(labelTexts[i]);
        lbl.setCharacterSize(18); // 🔑 Чуть крупнее подписи
        lbl.setFillColor(sf::Color(220, 220, 220));

        // 🔑 Выравниваем подписи справа от центра
        sf::FloatRect bounds = lbl.getLocalBounds();
        lbl.setOrigin(bounds.left + bounds.width, bounds.top + bounds.height / 2.0f);
        lbl.setPosition(240, y); // Правый край у X=240

        labels.push_back(lbl);

        // 🔑 Поля ввода по центру, чуть правее
        inputs.push_back(std::make_unique<TextInput>(font, L"", sf::Vector2f(260, y - 14), sf::Vector2f(200, 28)));
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
    for (size_t i = 0; i < typeButtons.size(); ++i) {
        typeButtons[i]->handleEvent(ev, mousePos);
        if (typeButtons[i]->isClicked()) {
            selectedType = static_cast<StructureType>(i);
            typeButtons[i]->resetClick();

            StructureParams p;
            p.type = selectedType;
            p.wallThickness = inputs[0]->getValue();
            p.compartmentVolumes = {inputs[1]->getValue()};
            p.numCompartments = static_cast<int>(inputs[2]->getValue());
            p.buriedFraction = std::clamp(inputs[3]->getValue(), 0.0, 1.0);

            if (onSelectCallback) onSelectCallback(p);
        }
    }

    for (auto& inp : inputs) inp->handleEvent(ev);

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