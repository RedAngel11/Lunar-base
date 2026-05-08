#include "SummaryScreen.h"

SummaryScreen::SummaryScreen(const sf::Font& f, SelectionData& sharedData)
    : font(f), data(sharedData) {

    // 🔑 Порядок: (текст, шрифт, размер)
    headerText.setFont(font);
    headerText.setString(sf::String(L"📋 Итоговая конфигурация"));
    headerText.setCharacterSize(26);
    headerText.setFillColor(sf::Color(255, 220, 100));
    sf::FloatRect hb = headerText.getLocalBounds();
    headerText.setOrigin(hb.left + hb.width/2, hb.top + hb.height/2);
    headerText.setPosition(400, 50);

    locationInfo.setFont(font);
    locationInfo.setCharacterSize(18);
    locationInfo.setFillColor(sf::Color::White);
    locationInfo.setPosition(50, 120);

    structureInfo.setFont(font);
    structureInfo.setCharacterSize(18);
    structureInfo.setFillColor(sf::Color::White);
    structureInfo.setPosition(50, 220);

    materialsInfo.setFont(font);
    materialsInfo.setCharacterSize(18);
    materialsInfo.setFillColor(sf::Color::White);
    materialsInfo.setPosition(50, 320);

    calculateButton = std::make_unique<Button>(
        font, sf::String(L"🚀 Начать расчёты"),
        sf::Vector2f(275, 480), sf::Vector2f(250, 60)
    );
}

void SummaryScreen::setOnCalculateCallback(std::function<void()> cb) {
    onCalculateCallback = std::move(cb);
}

void SummaryScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {
    calculateButton->handleEvent(ev, mousePos);
    if (calculateButton->isClicked()) {
        calculateButton->resetClick();
        if (onCalculateCallback) onCalculateCallback();
    }
}

void SummaryScreen::draw(sf::RenderTarget& target) const {
    // 🔑 Собираем строки через sf::String, избегаем конфликта std::string / wchar_t
    sf::String locStr = L"🌍 Локация: ";
    if (data.selectedLocation) {
        locStr += sf::String(data.selectedLocation->name);
        locStr += sf::String(L"\n   ☢️ Радиация: ") + std::to_wstring(data.selectedLocation->radiationLevel) + L" Зв/год";
        locStr += sf::String(L"\n   🌡️ Температура: ") + std::to_wstring(data.selectedLocation->temperatureMin) + L"°C ... " + std::to_wstring(data.selectedLocation->temperatureMax) + L"°C";
        locStr += sf::String(L"\n   🪨 Стабильность грунта: ") + std::to_wstring(data.selectedLocation->groundStability * 100.0) + L"%";
    } else {
        locStr += L"❌ Не выбрано";
    }

    sf::String strucStr = L"🏗️ Структура: ";
    if (data.selectedStructure) {
        switch (*data.selectedStructure) {
            case StructureType::SealedModule: strucStr += L"Герметичный модуль"; break;
            case StructureType::UndergroundBunker: strucStr += L"Подземный бункер"; break;
            case StructureType::InflatableDome: strucStr += L"Надувной купол"; break;
            case StructureType::RegolithPrinted: strucStr += L"3D-печать из реголита"; break;
            default: strucStr += L"Неизвестно"; break;
        }
    } else {
        strucStr += L"❌ Не выбрано";
    }

    sf::String matsStr = L"🧱 Материалы: ";
    if (!data.selectedMaterials.empty()) {
        for (auto m : data.selectedMaterials) {
            switch (m) {
                case MaterialType::RegolithConcrete: matsStr += L"Реголитовый бетон, "; break;
                case MaterialType::Aerogel: matsStr += L"Аэрогель, "; break;
                case MaterialType::TitaniumAlloy: matsStr += L"Титановый сплав, "; break;
                case MaterialType::PolymerFoam: matsStr += L"Полимерная пена, "; break;
                default: break;
            }
        }
    } else {
        matsStr += L"❌ Не выбрано";
    }

    // 🔑 mutable позволяет вызывать setString в const-методе
    locationInfo.setString(locStr);
    structureInfo.setString(strucStr);
    materialsInfo.setString(matsStr);

    target.draw(headerText);
    target.draw(locationInfo);
    target.draw(structureInfo);
    target.draw(materialsInfo);
    calculateButton->draw(target);
}