#include <iostream>
#include "src/core/Game.h" // Подключаем только заголовок игры
#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // Настройка консоли для корректного вывода кириллицы (опционально)
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    try {
        std::cout << "🚀 Запуск Lunar Base Simulator..." << std::endl;
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "❌ Fatal error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}