#include "LocationScreen.h"
#include <iostream>
#include <algorithm> // Для std::clamp

LocationScreen::LocationScreen(const sf::Font& f, const std::string& mapPath) : font(f) {
    if (!moonTexture.loadFromFile(mapPath)) {
        std::cerr << "⚠️ Warning: не удалось загрузить карту: " << mapPath << "\n";
        moonMap.setTextureRect(sf::IntRect(0, 0, 800, 600));
        moonMap.setColor(sf::Color(50, 50, 60));
    } else {
        moonMap.setTexture(moonTexture);
        // Центрируем карту при старте
        sf::Vector2u size = moonTexture.getSize();
        mapOffset = sf::Vector2f(400.f - (size.x * mapScale) / 2.f,
                                 300.f - (size.y * mapScale) / 2.f);
    }

    std::vector<LocationInfo> locations = {
        { "Oceanus Procellarum",  1.3, -175, 110, 0.80, 0.15f, 0.50f }, // Океан Бурь
        { "Aristarchus Crater",   1.1, -185, 120, 0.85, 0.22f, 0.42f }, // Кратер Аристарх
        { "Grimaldi Basin",       1.4, -170, 100, 0.75, 0.20f, 0.65f }, // Бассейн Гримальди
        { "Mare Frigoris",        1.2, -190, 100, 0.78, 0.55f, 0.18f }, // Море Холода
        { "Mare Imbrium",         1.3, -175, 115, 0.82, 0.35f, 0.30f }, // Море Дождей
        { "Plato Crater",         1.0, -180, 110, 0.88, 0.45f, 0.22f }, // Кратер Платон
        { "Mare Vaporum",         1.3, -180, 125, 0.80, 0.50f, 0.35f }, // Море Паров
        { "Mare Serenitatis",     1.1, -170, 125, 0.85, 0.68f, 0.28f }, // Море Ясности
        { "Mare Crisium",         1.2, -165, 115, 0.83, 0.82f, 0.30f }, // Море Кризисов
        { "Apennine Mountains",   1.5, -175, 130, 0.70, 0.55f, 0.42f }, // Апеннины
        { "Mare Tranquillitatis", 1.2, -170, 120, 0.85, 0.75f, 0.45f }, // Море Спокойствия
        { "Taurus-Littrow",       1.2, -190, 115, 0.88, 0.70f, 0.50f }, // Долина Таура-Литтроу
        { "Mare Fecunditatis",    1.2, -165, 115, 0.83, 0.80f, 0.55f }, // Море Изобилия
        { "Mare Nectaris",        1.1, -175, 120, 0.84, 0.72f, 0.62f }, // Море Нектара
        { "Mare Australe",        1.3, -180, 110, 0.78, 0.78f, 0.82f }, // Южное Море
        { "Tycho Crater",         1.1, -190, 110, 0.88, 0.62f, 0.78f }, // Кратер Тихо
        { "Shackleton Crater",    0.8, -230,  40, 0.92, 0.50f, 0.90f }, // Кратер Шеклтон
        { "Mare Nubium",          1.2, -172, 120, 0.82, 0.45f, 0.68f }, // Море Облаков
        { "Mare Humorum",         1.2, -175, 125, 0.80, 0.35f, 0.75f }, // Море Влажности
        { "Copernicus Crater",    1.0, -185, 115, 0.86, 0.38f, 0.48f }  // Кратер Коперник
    };

    sf::Vector2u texSize = moonTexture.getSize();
    for (const auto& loc : locations) {
        hotspots.push_back(std::make_unique<Hotspot>(font, loc, 25.0f, texSize));
    }

    infoPanel.setFont(font);
    infoPanel.setCharacterSize(14);
    infoPanel.setFillColor(sf::Color(220, 220, 220));
    infoPanel.setPosition(20, 520);
}

void LocationScreen::handleInput(const sf::Event& ev, const sf::Vector2i& mousePos) {
    sf::Vector2f mouse(mousePos.x, mousePos.y);

    if (ev.type == sf::Event::MouseWheelScrolled) {
        float zoomFactor = ev.mouseWheelScroll.delta > 0 ? 1.15f : 1.0f / 1.15f;
        mapScale *= zoomFactor;
        mapScale = std::clamp(mapScale, 0.15f, 4.0f);
        mapOffset = sf::Vector2f(400.f, 300.f) - (mapOffset - sf::Vector2f(400.f, 300.f)) * zoomFactor;
    }

    if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
        isDragging = true; dragStartMouse = mouse; dragStartOffset = mapOffset;
    }
    if (ev.type == sf::Event::MouseButtonReleased) isDragging = false;

    if (isDragging && ev.type == sf::Event::MouseMoved) {
        mapOffset += (mouse - dragStartMouse);

        // 🔑 ГРАНИЦЫ КАРТЫ (Не даем карте улететь в бесконечность)
        float mapW = moonTexture.getSize().x * mapScale;
        float mapH = moonTexture.getSize().y * mapScale;

        // Ограничиваем смещение, чтобы карта не выходила за окно больше чем на 50px
        mapOffset.x = std::clamp(mapOffset.x, 400.f - mapW + 50.f, 400.f - 50.f);
        mapOffset.y = std::clamp(mapOffset.y, 300.f - mapH + 50.f, 300.f - 50.f);
    }

    sf::Transform t;
    t.translate(mapOffset);
    // 🔑 ИСПРАВЛЕНИЕ: scale(float scaleX, float scaleY, float centerX, float centerY)
    t.scale(mapScale, mapScale, 400.f, 300.f);

    // Переводим экранные координаты мыши в локальные координаты карты
    sf::Vector2f mouseLocal = t.getInverse().transformPoint(mouse);

    // Проверка кликов по точкам
    for (auto& hs : hotspots) {
        hs->handleEvent(ev, mouseLocal);
        if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
            if (hs->isClicked()) {
                std::cout << "🎯 Выбрана локация: " << hs->getInfo().name << "\n";
                if (onSelectCallback) onSelectCallback(hs->getInfo());
                hs->resetClick();
            }
        }
    }
}

void LocationScreen::setOnSelectCallback(std::function<void(const LocationInfo&)> cb) {
    onSelectCallback = std::move(cb);
}


    // 🔑 ПЕРЕТАСКИВАНИЕ
    // if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
    //     isDragging = true;
    //     dragStartMouse = mouse;
    //     dragStartOffset = mapOffset;
    // }
    // if (ev.type == sf::Event::MouseButtonReleased) {
    //     isDragging = false;
    // }
    // if (isDragging && ev.type == sf::Event::MouseMoved) {
    //     mapOffset += (mouse - dragStartMouse);
    // }

    // 🔑 СОЗДАЁМ ТРАНСФОРМ (Сдвиг + Масштаб относительно центра)



void LocationScreen::draw(sf::RenderTarget& target) const {
    sf::Transform t;
    t.translate(mapOffset);
    // 🔑 ИСПРАВЛЕНИЕ: те же 4 аргумента
    t.scale(mapScale, mapScale, 400.f, 300.f);

    // Рисуем всё через трансформ
    target.draw(moonMap, t);
    for (const auto& hs : hotspots) {
        hs->draw(target, t);
    }

    infoPanel.setString(sf::String(L"💡 Колесико = зум, ЛКМ = двигать. Клик по точке для выбора"));
    target.draw(infoPanel);
}