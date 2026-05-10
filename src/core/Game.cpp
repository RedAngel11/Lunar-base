#include "Game.h"
#include "../screens/LocationScreen.h"
#include "../screens/StructureScreen.h"
#include "../screens/MaterialScreen.h"
#include "../screens/MainMenuScreen.h"
#include "../screens/SummaryScreen.h"
#include "../ui/SelectionButton.h"
#include <stdexcept>
#include <iostream>
#include <filesystem>

Game::Game() : window(sf::VideoMode(800, 600), "Lunar Base Simulator") {
    loadResources();
    switchScreen(ScreenType::MainMenu);
}

void Game::loadResources() {
    if (!font.loadFromFile("resources/Roboto.ttf"))
        throw std::runtime_error("Failed to load font!");
}

void Game::switchBuilderScreen(Tab tab) {
    switch (tab) {
        case Tab::Location: {
            auto screen = std::make_unique<LocationScreen>(font, "resources/moon_map.jpg");
            screen->setOnSelectCallback([this](const LocationInfo& info) {
                sharedData.selectedLocation = info;
                std::cout << "✓ Локация сохранена: " << info.name << "\n";
            });
            currentScreen = std::move(screen);
            break;
        }
        case Tab::Structure: {
            auto screen = std::make_unique<StructureScreen>(font);
            screen->setOnSelectCallback([this](StructureType type) {
                sharedData.selectedStructure = type;
                std::cout << "✓ Структура сохранена: тип #" << static_cast<int>(type) << "\n";
            });
            currentScreen = std::move(screen);
            break;
        }
        case Tab::Material: {  // 🔑 НОВОЕ: создаём экран материалов
            auto screen = std::make_unique<MaterialScreen>(font);
            screen->setOnSelectCallback([this](MaterialType type) {
                // Добавляем/удаляем материал из списка (можно сделать тоггл)
                auto it = std::find(sharedData.selectedMaterials.begin(),
                                   sharedData.selectedMaterials.end(), type);
                if (it != sharedData.selectedMaterials.end()) {
                    sharedData.selectedMaterials.erase(it);
                } else {
                    sharedData.selectedMaterials.push_back(type);
                }
                std::cout << "✓ Материал обновлён, всего выбрано: "
                          << sharedData.selectedMaterials.size() << "\n";
            });
            currentScreen = std::move(screen);
            break;
        }
        case Tab::Summary: {  // 🔑 НОВОЕ: создаём экран итогов
            auto screen = std::make_unique<SummaryScreen>(font, sharedData);
            screen->setOnCalculateCallback([this]() {
                if (sharedData.isReady()) {
                    std::cout << "🎯 Все данные собраны! Запускаем BaseCalculator...\n";
                    // Здесь будет вызов BaseCalculator::calculate(sharedData)
                } else {
                    std::cout << "⚠️ Заполните все вкладки перед расчётом!\n";
                }
            });
            currentScreen = std::move(screen);
            break;
        }
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