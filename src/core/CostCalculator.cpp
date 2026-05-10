#include "CostCalculator.h"
#include "stdexcept"

CostCalculator::CostResult CostCalculator::calculate(double totalVolume, double wallThickness, 
                                                    double density, double inSituFactor) {
    if (totalVolume <= 0.0 || wallThickness <= 0.0 || density <= 0.0)
        throw std::invalid_argument("Invalid volume, thickness or density");

    // Приближённая площадь поверхности куба
    double surfaceArea = 6.0 * std::pow(totalVolume, 2.0/3.0);
    double materialVolume = surfaceArea * wallThickness;
    double materialMass = materialVolume * density;
    
    double earthMass = materialMass * (1.0 - inSituFactor * 0.8);
    double localMass = materialMass - earthMass;
    
    double launchCost = earthMass * COST_PER_KG_LANDING;
    double constructionCost = localMass * LOCAL_CONSTRUCTION_COST;
    
    return {earthMass, localMass, launchCost, constructionCost, 0.0, launchCost + constructionCost};
}
