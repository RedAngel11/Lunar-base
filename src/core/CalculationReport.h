#pragma once
#include <string>
#include <vector>
#include <ostream>

struct CalculationReport {
    std::string locationName, materialName;
    double wallThickness, buriedFraction, totalVolume;
    
    double totalCost, earthLaunchMass, localMass;
    double estimatedLifespan, safetyScore;
    double radiationRisk, thermalStress;
    std::vector<std::string> recommendations;

    // Перегрузка оператора вывода (требование курса)
    friend std::ostream& operator<<(std::ostream& os, const CalculationReport& r);
};

// Шаблонная утилита для ограничения значений (требование курса: шаблоны)
template<typename T>
constexpr T clamp(T val, T minVal, T maxVal) {
    return (val < minVal) ? minVal : (val > maxVal) ? maxVal : val;
}