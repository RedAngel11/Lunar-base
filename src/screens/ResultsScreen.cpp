#include "ResultsScreen.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Форматирование wide-строк (убирает лишние нули)
std::wstring formatWNumber(double value) {
    std::wostringstream oss;
    oss << std::fixed << std::setprecision(2) << value; // 2 знака после запятой
    std::wstring str = oss.str();

    size_t dotPos = str.find(L'.');
    if (dotPos != std::wstring::npos) {
        size_t lastNonZero = str.find_last_not_of(L'0');
        if (lastNonZero != std::wstring::npos && lastNonZero > dotPos) {
            str.erase(lastNonZero + 1);
        }
        if (!str.empty() && str.back() == L'.') {
            str.pop_back();
        }
    }
    return str;
}

ResultsScreen::ResultsScreen(const sf::Font& f, const CalculationReport& rep)
    : font(f), report(rep) {

    if (resultTexture.loadFromFile("resources/dog.jpg")) {
        resultImage.setTexture(resultTexture);
        if (resultTexture.getSize().x > 220) {
            float scale = 250.0f / resultTexture.getSize().x;
            resultImage.setScale(scale, scale);
        }
        resultImage.setPosition(530, 70);
    }

    buildTextLines();
    std::cout << report << "\n";

    backButton = std::make_unique<Button>(
        font, sf::String(L"В главное меню"),
        sf::Vector2f(300, 540), sf::Vector2f(200, 40)
    );
}

void ResultsScreen::setOnBackCallback(std::function<void()> cb) { onBackCallback = std::move(cb); }

void ResultsScreen::buildTextLines() {
    sf::String fullText;

    fullText += sf::String(L"ИТОГОВЫЙ ОТЧЁТ\n") +
                sf::String(L" \n");
    fullText += sf::String(L"Локация: ") + sf::String::fromUtf8(report.locationName.begin(), report.locationName.end()) + sf::String(L"\n");
    fullText += sf::String(L"Материал: ") + sf::String::fromUtf8(report.materialName.begin(), report.materialName.end()) + sf::String(L"\n");

    fullText += sf::String(L"Стенка: ") + sf::String(formatWNumber(report.wallThickness)) +
                sf::String(L" м  |  Заглубление: ") + sf::String(std::to_wstring(int(report.buriedFraction * 100))) +
                sf::String(L" %\n");

    fullText += sf::String(L"Объём: ") + sf::String(std::to_wstring(int(report.totalVolume))) + sf::String(L" м³\n\n");

    fullText += sf::String(L" Стоимость: $") + sf::String(std::to_wstring(int(report.totalCost / 1e6))) + sf::String(L" млн\n");
    fullText += sf::String(L" Масса с Земли: ") + sf::String(std::to_wstring(int(report.earthLaunchMass))) + sf::String(L" кг\n");
    fullText += sf::String(L" Синтез на Луне: ") + sf::String(std::to_wstring(int(report.localMass))) + sf::String(L" кг\n\n");

    fullText += sf::String(L" Срок службы: ") + sf::String(formatWNumber(report.estimatedLifespan)) + sf::String(L" лет\n");
    fullText += sf::String(L" Безопасность: ") + sf::String(std::to_wstring(int(report.safetyScore))) + sf::String(L" / 100\n\n");

    fullText += sf::String(L"Рекомендации:\n");
    for (const auto& r : report.recommendations) {
        fullText += sf::String(L"• ") + sf::String::fromUtf8(r.begin(), r.end()) + sf::String(L"\n");
    }

    sf::Text txt;
    txt.setFont(font);
    txt.setString(fullText);
    txt.setCharacterSize(18);
    txt.setFillColor(sf::Color::White);
    txt.setPosition(30, 70);
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