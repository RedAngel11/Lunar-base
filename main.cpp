#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::cout << "Запуск Lunar Base Simulator..." << std::endl;
    // 1. Создаём окно (800x600 пикселей)
    sf::RenderWindow window(sf::VideoMode(800, 600), "Lunar Base Simulator");

    // 2. Загружаем шрифт и текстуру (файлы должны лежать в папке с проектом)
    sf::Font font;
    if (!font.loadFromFile("../assets/Roboto-Bold.ttf")) {
        std::cerr << "error font" << std::endl;
        return -1;
    }

    sf::Texture bgTexture;
    bgTexture.loadFromFile("../assets/oon3.jpg");
    sf::Sprite background(bgTexture);

    // 3. Создаём текст приветствия
    sf::Text welcomeText;
    welcomeText.setFont(font);
    welcomeText.setString("Welcome to my story!");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setPosition(150, 100);

    // 4. Создаём кнопку (прямоугольник + текст)
    sf::RectangleShape button(sf::Vector2f(200, 50));
    button.setFillColor(sf::Color::Blue);
    button.setPosition(300, 300);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString("Start Game");
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(340, 315);

    // 5. Игровой цикл
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Обработка клика мыши
            if (event.type == sf::Event::MouseButtonPressed) {
                if (button.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                                      sf::Mouse::getPosition(window).y)) {
                    std::cout << "miiuuu" << std::endl;
                    // Здесь вызов твоей основной логики игры
                }
            }
        }

        // Отрисовка
        window.clear();
        window.draw(background);
        window.draw(welcomeText);
        window.draw(button);
        window.draw(buttonText);
        window.display();
    }

    return 0;
}