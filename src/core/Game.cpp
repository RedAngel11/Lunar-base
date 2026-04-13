#include "Game.h"
#include "../screens/LocationScreen.h"
#include "../screens/StructureScreen.h"
#include "../screens/MaterialScreen.h"
#include "../screens/MainMenuScreen.h"
#include <stdexcept>
//#include <iostream>
//#include <filesystem>



Game::Game() : window(sf::VideoMode(800, 600), "Lunar Base Simulator") {
    loadResources();
    switchScreen(ScreenType::MainMenu);
}

void Game::loadResources() {
    if (!font.loadFromFile("resources/Roboto.ttf"))
        throw std::runtime_error("Failed to load font!");
}

// 🔑 Фабрика экранов конструктора
void Game::switchBuilderScreen(Tab tab) {
    switch (tab) {
        case Tab::Location:
            currentScreen = std::make_unique<LocationScreen>(font);
            break;
        case Tab::Structure:
            currentScreen = std::make_unique<StructureScreen>(font);
            break;
        case Tab::Material:
            currentScreen = std::make_unique<MaterialScreen>(font);
            break;
        case Tab::Summary:
            currentScreen = nullptr; // Пока заглушка для итогов
            break;
    }
}

void Game::switchScreen(ScreenType type) {
    switch (type) {
        case ScreenType::MainMenu: {
            auto menu = std::make_unique<MainMenuScreen>(font, "resources/oon3.jpg");
            menu->setOnStartCallback([this]() { switchScreen(ScreenType::Builder); });
            currentScreen = std::move(menu);
            topPanel = nullptr;
            break;
        }
        case ScreenType::Builder: {
            topPanel = std::make_unique<TopPanel>(font, 800.0f);
            lastActiveTab = Tab::Location; // Сбрасываем на первую вкладку
            switchBuilderScreen(Tab::Location); // Инициализируем первый экран
            break;
        }
    }
    currentScreenType = type;
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (currentScreen) currentScreen->handleInput(event, mousePos);

            if (topPanel) {
                topPanel->handleEvent(event, mousePos);
                // 🔑 ГЛАВНОЕ ИСПРАВЛЕНИЕ: отслеживаем клик по вкладке
                if (topPanel->getActiveTab() != lastActiveTab) {
                    lastActiveTab = topPanel->getActiveTab();
                    switchBuilderScreen(lastActiveTab);
                }
            }
        }

        window.clear(sf::Color(20, 20, 30));
        if (currentScreen) currentScreen->draw(window);
        if (topPanel) topPanel->draw(window);
        window.display();
    }
}