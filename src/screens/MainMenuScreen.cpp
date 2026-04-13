#include "MainMenuScreen.h"
#include <iostream>

MainMenuScreen::MainMenuScreen(const sf::Font& font, const std::string& bgPath) {
    if (!bgTexture.loadFromFile(bgPath)) {
        std::cerr << "⚠️ Warning: не удалось загрузить фон: " << bgPath << "\n";
        background.setTextureRect(sf::IntRect(0, 0, 800, 800));
        background.setColor(sf::Color(30, 30, 50));
    } else {
        background.setTexture(bgTexture);
        background.setScale(
            800.0f / bgTexture.getSize().x,
            800.0f / bgTexture.getSize().y
        );
    }

    title.setFont(font);
    title.setString(sf::String(L"Lunar Base Simulator"));
    title.setCharacterSize(32);
    title.setFillColor(sf::Color::White);

    // 🔑 1. Получаем границы текста в его локальной системе координат
    sf::FloatRect textBounds = title.getLocalBounds();

    // 🔑 2. Сдвигаем "якорь" (origin) в геометрический центр текста
    title.setOrigin(textBounds.left + textBounds.width / 2.0f,
                    textBounds.top + textBounds.height / 2.0f);

    // 🔑 3. Размещаем якорь по центру окна (800/2) и по нужной высоте (80)
    title.setPosition(800.0f / 2.0f, 80.0f);

    startButton = std::make_unique<Button>(
        font,
        sf::String(L"Начать игру"),
        sf::Vector2f(300, 300),
        sf::Vector2f(200, 50)
    );
}

void MainMenuScreen::setOnStartCallback(std::function<void()> callback) {
    onStartCallback = std::move(callback);
}

// 🔑 ИСПРАВЛЕНО: используем переданный mousePos
void MainMenuScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {
    startButton->handleEvent(ev, mousePos);

    if (startButton->isClicked()) {
        startButton->resetClick();
        if (onStartCallback) {
            onStartCallback();
        } else {
            std::cout << "❌ Колбэк НЕ установлен!\n";
        }
    }
}

void MainMenuScreen::draw(sf::RenderTarget& target) const {
    target.draw(background);
    target.draw(title);
    startButton->draw(target); // 🔑 ИСПРАВЛЕНА ОПЕЧАТКА (было startButton->})
}