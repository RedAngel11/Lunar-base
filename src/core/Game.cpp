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
#include "../core/BaseCalculator.h"
#include "../screens/ResultsScreen.h"

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
                std::cout << "🌍 Выбрана локация: " << info.name << "\n";
            });
            currentScreen = std::move(screen);
            break;
        }
        case Tab::Structure: {
            auto screen = std::make_unique<StructureScreen>(font);
            screen->setOnSelectCallback([this](const StructureParams& p) {
                sharedData.structureParams = p;
                sharedData.selectedStructure = p.type;

                std::string typeName = "Unknown";
                switch (p.type) {
                    case StructureType::SealedModule: typeName = "Герметичный модуль"; break;
                    case StructureType::UndergroundBunker: typeName = "Подземный бункер"; break;
                    case StructureType::InflatableDome: typeName = "Надувной купол"; break;
                    case StructureType::RegolithPrinted: typeName = "3D-печать из реголита"; break;
                }
                std::cout << "🏗️ Структура: " << typeName
                          << " | Объём=" << p.compartmentVolumes[0]
                          << " | Стенка=" << p.wallThickness << "м\n";
            });
            currentScreen = std::move(screen);
            break;
        }
        case Tab::Material: {
            auto screen = std::make_unique<MaterialScreen>(font);
            // 🔑 Безопасный захват: сохраняем сырой указатель до перемещения unique_ptr
            auto* screenPtr = screen.get();
            screen->setOnSelectCallback([this, screenPtr](MaterialType type) {
                sharedData.selectedMaterials.clear();
                sharedData.selectedMaterials.push_back(type);

                std::string matName = "Неизвестно";
                if (type == MaterialType::RegolithConcrete) matName = "Реголитовый бетон";
                else if (type == MaterialType::Aerogel) matName = "Аэрогель";
                else if (type == MaterialType::TitaniumAlloy) matName = "Титановый сплав";
                else if (type == MaterialType::PolymerFoam) matName = "Полимерная пена";
                else if (type == MaterialType::None) matName = "Пользовательский материал";

                std::cout << "🧱 Выбран материал: " << matName << "\n";

                if (type == MaterialType::None && screenPtr) {
                    sharedData.customMat = screenPtr->getCustomParams();
                }
            });
            currentScreen = std::move(screen);
            break;
        }
        case Tab::Summary: {
            auto screen = std::make_unique<SummaryScreen>(font, sharedData);
            screen->setOnCalculateCallback([this]() {
                try {
                    CalculationReport report = BaseCalculator::calculate(sharedData);
                    auto results = std::make_unique<ResultsScreen>(font, report);

                    // 🔑 ВОЗВРАТ В ГЛАВНОЕ МЕНЮ
                    results->setOnBackCallback([this]() {
                        switchScreen(ScreenType::MainMenu);
                    });

                    currentScreen = std::move(results);
                    topPanel = nullptr;
                } catch (const std::exception& e) {
                    std::cerr << "Ошибка расчёта: " << e.what() << "\n";
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
