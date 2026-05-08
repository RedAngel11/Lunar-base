#pragma once
#include <SFML/Graphics.hpp>
#include "../ui/TopPanel.h"
#include "../screens/IScreen.h"
#include "SelectionData.h"
#include "../screens/SummaryScreen.h"
#include <memory>

enum class ScreenType { MainMenu, Builder };

class Game {
    sf::RenderWindow window;
    sf::Font font;
    std::unique_ptr<IScreen> currentScreen;
    ScreenType currentScreenType = ScreenType::MainMenu;

    std::unique_ptr<TopPanel> topPanel;
    Tab lastActiveTab = Tab::Location; // Отслеживаем смену вкладки

    void switchBuilderScreen(Tab tab); // Новый метод
    void loadResources();
    SelectionData sharedData; // Единое хранилище выбора
    void updateSummaryText(); // Обновить текст в SummaryScreen

public:
    Game();
    void run();
    void switchScreen(ScreenType type);
};