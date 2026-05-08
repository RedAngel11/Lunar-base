#pragma once
#include "Button.h"
#include <string>

class SelectionButton : public Button {
    bool isSelected = false;
    sf::Color selectedColor{100, 200, 100};
    sf::Color normalColor{60, 60, 70};

public:
    SelectionButton(const sf::Font& font, const sf::String& label, 
                   sf::Vector2f pos, sf::Vector2f size)
        : Button(font, label, pos, size) {
        // Можно кастомизировать внешний вид
    }
    
    void toggleSelect() { 
        isSelected = !isSelected; 
        // Здесь можно менять цвет, но Button инкапсулирует цвета,
        // поэтому в реальном проекте лучше добавить метод setColor в Button
    }
    
    [[nodiscard]] bool getSelected() const { return isSelected; }
    void setSelected(bool val) { isSelected = val; }
};