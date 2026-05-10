#include "ResultsScreen.h"
#include <iostream>
#include <sstream>

ResultsScreen::ResultsScreen(const sf::Font& f, const CalculationReport& rep)
    : font(f), report(rep) {
    buildTextLines();
    std::cout << report << "\n";

    backButton = std::make_unique<Button>(
        font, sf::String(L"⬅️ Назад к конструктору"),
        sf::Vector2f(300, 540), sf::Vector2f(200, 40)
    );
}

void ResultsScreen::setOnBackCallback(std::function<void()> cb) { onBackCallback = std::move(cb); }

void ResultsScreen::buildTextLines() {
    sf::String fullText;
    fullText += sf::String(L"📊 ОТЧЁТ ПО БАЗЕ\n");
    fullText += sf::String(L"🌍 ") + sf::String(report.locationName) + sf::String(L" | 🧱 ") + sf::String(report.materialName) + sf::String(L"\n");
    fullText += sf::String(L"📏 Стенка: ") + std::to_wstring(report.wallThickness) + sf::String(L"м | 🕳️ Закопано: ") + std::to_wstring(int(report.buriedFraction*100)) + sf::String(L"%\n");
    fullText += sf::String(L"📦 Объём: ") + std::to_wstring(int(report.totalVolume)) + sf::String(L"м³\n\n");
    fullText += sf::String(L"💰 Стоимость: $") + std::to_wstring(int(report.totalCost/1e6)) + sf::String(L" млн\n");
    fullText += sf::String(L"🚀 Доставка с Земли: ") + std::to_wstring(int(report.earthLaunchMass)) + sf::String(L" кг\n");
    fullText += sf::String(L"🏗️ Синтез на Луне: ") + std::to_wstring(int(report.localMass)) + sf::String(L" кг\n\n");
    fullText += sf::String(L"⏳ Срок службы: ") + std::to_wstring(int(report.estimatedLifespan*10)/10.0) + sf::String(L" лет\n");
    fullText += sf::String(L"☢️ Радиация: ") + std::to_wstring(int(report.radiationRisk*10)/10.0) + sf::String(L" | 🌡️ Стресс: ") + std::to_wstring(int(report.thermalStress)) + sf::String(L"\n\n");
    fullText += sf::String(L"🛡️ Безопасность: ") + std::to_wstring(int(report.safetyScore)) + sf::String(L"/100\n\n");
    fullText += sf::String(L"💡 Рекомендации:\n");
    for (const auto& r : report.recommendations) {
        fullText += sf::String(L"• ") + sf::String(r) + sf::String(L"\n");
    }

    sf::Text txt;
    txt.setFont(font);
    txt.setString(fullText);
    txt.setCharacterSize(16);
    txt.setFillColor(sf::Color::White);
    txt.setPosition(40, 60);
    textLines.push_back(txt);
}

void ResultsScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {
    backButton->handleEvent(ev, mousePos);
    if (backButton->isClicked()) {
        backButton->resetClick();
        if (onBackCallback) onBackCallback();
    }
}

void ResultsScreen::draw(sf::RenderTarget& target) const {
    target.draw(textLines[0]);
    backButton->draw(target);
}