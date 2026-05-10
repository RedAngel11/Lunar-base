#pragma once
#include "CalculationReport.h"
#include "CostCalculator.h"
#include "SelectionData.h"
#include <cmath>
#include <stdexcept>

class BaseCalculator {
public:
    struct MaterialProps {
        std::string name;
        double density, thermalConductivity, radiationShielding, durability, inSituProducibility;
    };

    static CalculationReport calculate(const SelectionData& data);
private:
    static MaterialProps getMaterialProps(MaterialType type);
};