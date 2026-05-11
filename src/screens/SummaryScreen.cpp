#include "SummaryScreen.h"
#include <string>
#include <sstream>
#include <iomanip>

SummaryScreen::SummaryScreen(const sf::Font& f, SelectionData& sharedData)
    : font(f), data(sharedData) {

    title.setFont(font);
    title.setString(sf::String(L"ПРОВЕРКА ДАННЫХ"));
    title.setCharacterSize(30);
    title.setFillColor(sf::Color(255, 200, 50));
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
    title.setPosition(400.0f, 70.0f);

    calculateButton = std::make_unique<Button>(
        font, sf::String(L"🚀 НАЧАТЬ РАСЧЁТЫ"),
        sf::Vector2f(250, 480), sf::Vector2f(300, 60)
    );

    updateDataDisplay();
}

void SummaryScreen::setOnCalculateCallback(std::function<void()> cb) {
    onCalculateCallback = std::move(cb);
}

std::string SummaryScreen::getStructureName(StructureType type) const {
    switch (type) {
        case StructureType::SealedModule: return "Герметичный модуль";
        case StructureType::UndergroundBunker: return "Подземный бункер";
        case StructureType::InflatableDome: return "Надувной купол";
        case StructureType::RegolithPrinted: return "3D-печать (Реголит)";
        default: return "Не выбрано";
    }
}

std::string SummaryScreen::getMaterialName(MaterialType type) const {
    switch (type) {
        case MaterialType::RegolithConcrete: return "Реголитовый бетон";
        case MaterialType::Aerogel: return "Аэрогель";
        case MaterialType::TitaniumAlloy: return "Титановый сплав";
        case MaterialType::PolymerFoam: return "Полимерная пена";
        case MaterialType::None: return "Свой материал";
        default: return "Не выбрано";
    }
}

// 🔑 ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ: Красивое форматирование чисел
std::string formatNumber(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value; // 2 знака после запятой
    std::string str = oss.str();

    // Убираем лишние нули в конце (например, 0.50 -> 0.5)
    size_t dotPos = str.find('.');
    if (dotPos != std::string::npos) {
        size_t lastNonZero = str.find_last_not_of('0');
        if (lastNonZero != std::string::npos && lastNonZero > dotPos) {
            str.erase(lastNonZero + 1);
        }
        // Если после точки ничего не осталось, убираем и точку
        if (str.back() == '.') {
            str.pop_back();
        }
    }
    return str;
}

void SummaryScreen::updateDataDisplay() {
    labelLines.clear();
    valueLines.clear();

    float y = 140;

    auto addRow = [&](const std::string& label, const std::string& value) {
        sf::Text lbl(sf::String::fromUtf8(label.begin(), label.end()), font, 20);
        lbl.setFillColor(sf::Color(150, 150, 150));

        sf::Text val(sf::String::fromUtf8(value.begin(), value.end()), font, 20);
        val.setFillColor(sf::Color::White);

        sf::FloatRect lBounds = lbl.getLocalBounds();
        lbl.setOrigin(lBounds.left + lBounds.width, lBounds.top + lBounds.height / 2.0f);
        lbl.setPosition(390, y);

        val.setPosition(410, y - 10);

        labelLines.push_back(lbl);
        valueLines.push_back(val);
        y += 45;
    };

    // 1. Локация
    std::string locName = data.selectedLocation ? data.selectedLocation->name : "Не выбрано";
    addRow("Локация:", locName);

    // 2. Структура
    std::string structName = getStructureName(data.selectedStructure.value_or(StructureType::None));
    addRow("Тип базы:", structName);

    // 3. Параметры структуры (теперь с красивым форматированием)
    double vol = 0.0;
    if (!data.structureParams.compartmentVolumes.empty()) vol = data.structureParams.compartmentVolumes[0];

    addRow("Толщина стенок:", formatNumber(data.structureParams.wallThickness) + " м");
    addRow("Общий объём:", formatNumber(vol) + " м3");
    addRow("Заглубление:", formatNumber(data.structureParams.buriedFraction * 100) + " %");

    // 4. Материал
    std::string matName = "Не выбрано";
    if (!data.selectedMaterials.empty()) {
        matName = getMaterialName(data.selectedMaterials.front());
    }
    addRow("Материал:", matName);
}

void SummaryScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {
    calculateButton->handleEvent(ev, mousePos);
    if (calculateButton->isClicked()) {
        calculateButton->resetClick();
        if (onCalculateCallback) onCalculateCallback();
    }
}

void SummaryScreen::draw(sf::RenderTarget& target) const {
    target.draw(title);
    for (const auto& lbl : labelLines) target.draw(lbl);
    for (const auto& val : valueLines) target.draw(val);
    calculateButton->draw(target);
}