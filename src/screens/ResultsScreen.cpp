#include "ResultsScreen.h"
#include <iostream>
#include <sstream>

ResultsScreen::ResultsScreen(const sf::Font& f, const CalculationReport& rep)
    : font(f), report(rep) {

    if (resultTexture.loadFromFile("resources/cat.png")) {
        resultImage.setTexture(resultTexture);
        if (resultTexture.getSize().x > 220) {
            float scale = 220.0f / resultTexture.getSize().x;
            resultImage.setScale(scale, scale);
        }
        resultImage.setPosition(530, 60); // Чуть правее и ниже
    }

    buildTextLines();
    std::cout << report << "\n";

    backButton = std::make_unique<Button>(
        font, sf::String(L"Вернуться в меню"),
        sf::Vector2f(300, 540), sf::Vector2f(200, 40)
    );
}

void ResultsScreen::setOnBackCallback(std::function<void()> cb) { onBackCallback = std::move(cb); }

void ResultsScreen::buildTextLines() {
    sf::String fullText;

    fullText += sf::String(L"📊 ИТОГОВЫЙ ОТЧЁТ\n");
    fullText += sf::String(L"━━━━━━━━━━━━━━━━━━━━\n");

    fullText += sf::String(L"🌍 Локация: ") + sf::String::fromUtf8(report.locationName.begin(), report.locationName.end()) + L"\n";
    fullText += sf::String(L"🧱 Материал: ") + sf::String::fromUtf8(report.materialName.begin(), report.materialName.end()) + L"\n";
    fullText += sf::String(L"📏 Стенка: ") + std::to_wstring(report.wallThickness) + L" м  |  🕳️ Заглубление: " + std::to_wstring(int(report.buriedFraction * 100)) + L" %\n";
    fullText += sf::String(L"📦 Объём: ") + std::to_wstring(int(report.totalVolume)) + L" м³\n\n";

    fullText += sf::String(L"💰 Стоимость: $") + std::to_wstring(int(report.totalCost / 1e6)) + L" млн\n";
    fullText += sf::String(L"🚀 Масса с Земли: ") + std::to_wstring(int(report.earthLaunchMass)) + L" кг\n";
    fullText += sf::String(L"🏗️ Синтез на Луне: ") + std::to_wstring(int(report.localMass)) + L" кг\n\n";

    fullText += sf::String(L"⏳ Срок службы: ") + std::to_wstring(int(report.estimatedLifespan * 10) / 10.0) + L" лет\n";
    fullText += sf::String(L"🛡️ Безопасность: ") + std::to_wstring(int(report.safetyScore)) + L" / 100\n\n";

    fullText += sf::String(L"💡 Рекомендации:\n");
    for (const auto& r : report.recommendations) {
        fullText += sf::String(L"• ") + sf::String::fromUtf8(r.begin(), r.end()) + L"\n";
    }

    sf::Text txt;
    txt.setFont(font);
    txt.setString(fullText);
    txt.setCharacterSize(16); // 🔑 Чуть крупнее шрифт отчёта
    txt.setFillColor(sf::Color::White);
    txt.setPosition(30, 50);
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
    if (resultTexture.getSize().x > 0) {
        target.draw(resultImage);
    }
}