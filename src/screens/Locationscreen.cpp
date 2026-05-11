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

    // 10 ЛОКАЦИЙ (Названия на английском + координаты)
    std::vector<LocationInfo> locations = {
        { "Oceanus Procellarum", 1.3, -175, 110, 0.80, 0.15f, 0.50f },
        { "Mare Tranquillitatis", 1.2, -170, 120, 0.85, 0.62f, 0.38f },
        { "Tycho Crater", 1.1, -190, 110, 0.88, 0.60f, 0.75f },
        { "Shackleton Crater", 0.8, -230,  40, 0.92, 0.50f, 0.92f },
        { "Lunar Highlands", 1.5, -180, 130, 0.70, 0.75f, 0.55f },
        { "Mare Imbrium", 1.4, -180, 115, 0.75, 0.35f, 0.30f },
        { "Mare Serenitatis", 1.1, -170, 125, 0.82, 0.55f, 0.35f },
        { "Copernicus Crater", 1.0, -190, 120, 0.85, 0.40f, 0.50f },
        { "Apennine Mtns", 1.6, -180, 135, 0.65, 0.50f, 0.45f },
        { "Taurus-Littrow", 1.2, -190, 115, 0.88, 0.65f, 0.42f }
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