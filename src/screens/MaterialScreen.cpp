#include "MaterialScreen.h"
#include "../ui/TextInput.h"

MaterialScreen::MaterialScreen(const sf::Font& f) : font(f) {
    title.setFont(font);
    title.setString(sf::String(L"Выбор материалов"));
    title.setCharacterSize(30);
    title.setFillColor(sf::Color::White);
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
    title.setPosition(400.0f, 70.0f);

    std::vector<std::pair<sf::String, MaterialType>> options = {
        {sf::String(L"Реголитовый бетон"), MaterialType::RegolithConcrete},
        {sf::String(L"Аэрогель"), MaterialType::Aerogel},
        {sf::String(L"Титановый сплав"), MaterialType::TitaniumAlloy},
        {sf::String(L"Полимерная пена"), MaterialType::PolymerFoam}
    };

    float y = 130;
    for (const auto& [label, type] : options) {
        // 🔑 Центрируем кнопки
        materialButtons.push_back({std::make_unique<Button>(font, label, sf::Vector2f(250, y), sf::Vector2f(300, 40)), type});
        y += 50;
    }

    customBtn = std::make_unique<Button>(font, sf::String(L"РАЗРАБОТАТЬ СВОЙ МАТЕРИАЛ"), sf::Vector2f(250, y), sf::Vector2f(300, 40));
    y += 60;

    // 🔑 Поля для кастомного материала тоже по центру
    customInputs.push_back(std::make_unique<TextInput>(font, L"Плотность (кг/м3)", sf::Vector2f(250, y), sf::Vector2f(300, 28)));
    customInputs.push_back(std::make_unique<TextInput>(font, L"Теплопроводность", sf::Vector2f(250, y + 38), sf::Vector2f(300, 28)));
    customInputs.push_back(std::make_unique<TextInput>(font, L"Защита от радиации", sf::Vector2f(250, y + 76), sf::Vector2f(300, 28)));
    customInputs.push_back(std::make_unique<TextInput>(font, L"Срок службы (лет)", sf::Vector2f(250, y + 114), sf::Vector2f(300, 28)));
    customInputs.push_back(std::make_unique<TextInput>(font, L"Доля синтеза на Луне", sf::Vector2f(250, y + 152), sf::Vector2f(300, 28)));
    customInputs.push_back(std::make_unique<TextInput>(font, L"Время разработки (лет)", sf::Vector2f(250, y + 190), sf::Vector2f(300, 28)));

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