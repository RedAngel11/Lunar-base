#pragma once
#include <cmath>

class CostCalculator {
public:
    static constexpr double COST_PER_KG_LANDING = 150000.0;
    static constexpr double LOCAL_CONSTRUCTION_COST = 500.0;
    static constexpr double COMPLEXITY_PENALTY = 500000.0;
    static constexpr int BASE_COMPARTMENTS = 3;

    struct CostResult {
        double earthLaunchMass = 0.0;
        double localMass = 0.0;
        double launchCost = 0.0;
        double constructionCost = 0.0;
        double complexityCost = 0.0;
        double totalCost = 0.0;
    };

    static CostResult calculate(double totalVolume, double wallThickness, 
                               double density, double inSituFactor);
};