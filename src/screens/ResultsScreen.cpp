#include "ResultsScreen.h"
#include <iostream>
#include <sstream>

ResultsScreen::ResultsScreen(const sf::Font& f, const CalculationReport& rep)
    : font(f), report(rep) {

    // 🔑 ЗАГРУЗКА КАРТИНКИ
    if (resultTexture.loadFromFile("resources/cat.jpg")) {
        resultImage.setTexture(resultTexture);
        // Масштабируем, если картинка огромная
        if (resultTexture.getSize().x > 250) {
            float scale = 250.0f / resultTexture.getSize().x;
            resultImage.setScale(scale, scale);
        }
        // Ставим картинку справа сверху
        resultImage.setPosition(520, 50);
    }

    buildTextLines();
    std::cout << report << "\n"; // Дублируем в консоль

    // Кнопка "Назад"
    backButton = std::make_unique<Button>(
        font, sf::String(L"⬅️ Вернуться в меню"),
        sf::Vector2f(300, 540), sf::Vector2f(200, 40)
    );
}

void ResultsScreen::setOnBackCallback(std::function<void()> cb) { onBackCallback = std::move(cb); }

void ResultsScreen::buildTextLines() {
    // Используем sf::String для корректной сборки русского текста
    sf::String fullText;

    // Заголовки и статичные надписи пишем через L"..." (Wide String)
    fullText += sf::String(L"📊 ИТОГОВЫЙ ОТЧЁТ\n");
    fullText += sf::String(L"━━━━━━━━━━━━━━━━━━━━\n");

    // Динамические данные (из отчёта) конвертируем из UTF-8
    fullText += sf::String(L"🌍 Локация: ") + sf::String::fromUtf8(report.locationName.begin(), report.locationName.end()) + L"\n";
    fullText += sf::String(L"🧱 Материал: ") + sf::String::fromUtf8(report.materialName.begin(), report.materialName.end()) + L"\n";
    fullText += sf::String(L"📏 Толщина стенки: ") + std::to_wstring(report.wallThickness) + L" м\n";
    fullText += sf::String(L"🕳️ Заглубление: ") + std::to_wstring(int(report.buriedFraction * 100)) + L" %\n";
    fullText += sf::String(L"📦 Объём базы: ") + std::to_wstring(int(report.totalVolume)) + L" м³\n\n";

    fullText += sf::String(L"💰 Общая стоимость: $") + std::to_wstring(int(report.totalCost / 1e6)) + L" млн\n";
    fullText += sf::String(L"🚀 Масса с Земли: ") + std::to_wstring(int(report.earthLaunchMass)) + L" кг\n";
    fullText += sf::String(L"🏗️ Синтез на Луне: ") + std::to_wstring(int(report.localMass)) + L" кг\n\n";

    fullText += sf::String(L"⏳ Прогноз срока службы: ") + std::to_wstring(int(report.estimatedLifespan * 10) / 10.0) + L" лет\n";
    fullText += sf::String(L"🛡️ Уровень безопасности: ") + std::to_wstring(int(report.safetyScore)) + L" / 100\n\n";

    fullText += sf::String(L"💡 Рекомендации инженеров:\n");
    for (const auto& r : report.recommendations) {
        // Конвертируем рекомендации из UTF-8
        fullText += sf::String(L"• ") + sf::String::fromUtf8(r.begin(), r.end()) + L"\n";
    }

    // Создаём текстовый объект SFML
    sf::Text txt;
    txt.setFont(font);
    txt.setString(fullText);
    txt.setCharacterSize(15); // Чуть уменьшил шрифт, чтобы всё влезло с картинкой
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
    // Рисуем текст
    target.draw(textLines[0]);
    // Рисуем кнопку
    backButton->draw(target);

    // 🔑 Рисуем картинку (если она загрузилась)
    if (resultTexture.getSize().x > 0) {
        target.draw(resultImage);
    }
}