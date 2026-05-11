#include "BaseCalculator.h"
#include <sstream>
#include <algorithm>

BaseCalculator::MaterialProps BaseCalculator::getMaterialProps(MaterialType type) {
    switch (type) {
        case MaterialType::RegolithConcrete: return {"Regolith Concrete", 2200, 1.2, 0.6, 25.0, 0.9};
        case MaterialType::Aerogel:          return {"Aerogel", 150, 0.02, 0.3, 15.0, 0.1};
        case MaterialType::TitaniumAlloy:    return {"Titanium Alloy", 4500, 22.0, 0.9, 40.0, 0.0};
        case MaterialType::PolymerFoam:      return {"Polymer Foam", 300, 0.05, 0.4, 20.0, 0.3};
        default: throw std::invalid_argument("Unknown material type");
    }
}

BaseCalculator::MaterialProps BaseCalculator::getCustomMaterialProps(const CustomMaterialParams& p) {
    return {"Custom Material", p.density, p.thermalConductivity, p.radiationShielding,
            p.durability, p.inSituProducibility};
}

CalculationReport BaseCalculator::calculate(const SelectionData& data) {
    if (!data.isReady()) throw std::runtime_error("Incomplete selection data");
    if (data.structureParams.wallThickness <= 0.0 || data.structureParams.numCompartments <= 0)
        throw std::invalid_argument("Invalid structure parameters");

    const auto& loc = *data.selectedLocation;
    const auto& mat = (data.selectedMaterials.front() == MaterialType::None)
                      ? getCustomMaterialProps(data.customMat)
                      : getMaterialProps(data.selectedMaterials.front());
    const auto& sp = data.structureParams;

    double totalVolume = 0.0;
    for (double v : sp.compartmentVolumes) totalVolume += v;

    // 🔑 Модификаторы в зависимости от типа структуры
    double structRadiationMod = 1.0;
    double structThermalMod = 1.0;
    double structDurabilityMod = 1.0;
    double effectiveBurial = sp.buriedFraction;

    switch (sp.type) {
        case StructureType::UndergroundBunker:
            effectiveBurial += 0.4; // Бункер автоматически заглублён
            structRadiationMod = 0.6;
            structThermalMod = 0.7;
            structDurabilityMod = 1.2;
            break;
        case StructureType::InflatableDome:
            structRadiationMod = 1.4; // Хуже защищает от радиации
            structThermalMod = 1.2;
            structDurabilityMod = 0.7; // Меньше срок службы
            effectiveBurial *= 0.5;
            break;
        case StructureType::RegolithPrinted:
            structRadiationMod = 0.8;
            structThermalMod = 0.9;
            structDurabilityMod = 1.3;
            effectiveBurial += 0.2;
            break;
        default: // SealedModule
            break;
    }

    // 1. Радиационный фактор
    double solarFactor = 1.0 + (350.0 / 700.0) * 0.3;
    double effectiveRadiation = loc.radiationLevel * solarFactor * structRadiationMod;
    double regolithShield = 5.0 * std::clamp(effectiveBurial, 0.0, 1.0);
    double effShielding = sp.wallThickness * mat.radiationShielding + regolithShield;
    double radiationRisk = clamp(effectiveRadiation / (effShielding + 0.01), 0.0, 100.0);

    // 2. Термический стресс
    double tempRange = loc.temperatureMax - loc.temperatureMin;
    double insulFactor = 1.0 / (mat.thermalConductivity + 0.01);
    double thermalStress = clamp(tempRange * (1.0 - insulFactor * 0.05) * (1.0 - effectiveBurial * 0.6) * structThermalMod, 0.0, 1000.0);

    // 3. Срок службы
    double degradation = radiationRisk * 0.15 + thermalStress * 0.03;
    double lifespan = clamp((mat.durability * structDurabilityMod) / (1.0 + degradation), 0.0, 50.0);

    // 4. Безопасность
    double rScore = std::max(0.0, 100.0 - radiationRisk * 40.0);
    double tScore = std::max(0.0, 100.0 - thermalStress * 0.2);
    double sScore = std::min(100.0, 50.0 + sp.wallThickness * 10.0 + effectiveBurial * 30.0);
    double safety = rScore * 0.4 + tScore * 0.3 + sScore * 0.3;

    // 5. Стоимость
    auto costRes = CostCalculator::calculate(totalVolume, sp.wallThickness, mat.density, mat.inSituProducibility);
    double complexityPenalty = sp.numCompartments > CostCalculator::BASE_COMPARTMENTS ?
        (sp.numCompartments - CostCalculator::BASE_COMPARTMENTS) * CostCalculator::COMPLEXITY_PENALTY : 0.0;
    costRes.totalCost += complexityPenalty;

    // Доп. стоимость для сложных типов
    if (sp.type == StructureType::UndergroundBunker) costRes.totalCost *= 1.15; // Дороже рытьё
    if (sp.type == StructureType::InflatableDome) costRes.totalCost *= 0.85;     // Дешевле развёртывание

    // Cost for custom material development
    if (data.selectedMaterials.front() == MaterialType::None) {
        costRes.totalCost += data.customMat.devTimeYears * 1000000.0;
    }

    std::vector<std::string> recs;
    if (lifespan < 10.0) recs.push_back("Lifespan is low. Increase wall thickness or add regolith.");
    if (radiationRisk > 5.0) recs.push_back("High radiation risk. Recommend burial or aerogel.");
    if (thermalStress > 200.0) recs.push_back("High thermal stress. Increase insulation materials.");
    if (recs.empty()) recs.push_back("Configuration is optimal for selected conditions.");

    return CalculationReport{
        loc.name, mat.name, sp.wallThickness, effectiveBurial, totalVolume,
        costRes.totalCost, costRes.earthLaunchMass, costRes.localMass,
        lifespan, safety, radiationRisk, thermalStress, std::move(recs)
    };
}

std::ostream& operator<<(std::ostream& os, const CalculationReport& r) {
    os << "LUNAR BASE REPORT\n"
       << "=================\n"
       << "Location: " << r.locationName << "\n"
       << "Material: " << r.materialName << "\n"
       << "Wall Thickness: " << r.wallThickness << " m\n"
       << "Total Volume: " << r.totalVolume << " m3\n\n"
       << "Total Cost: $" << (r.totalCost/1e6) << " million\n"
       << "Earth Launch Mass: " << r.earthLaunchMass << " kg\n"
       << "Local Mass: " << r.localMass << " kg\n\n"
       << "Estimated Lifespan: " << r.estimatedLifespan << " years\n"
       << "Safety Score: " << static_cast<int>(r.safetyScore) << "/100\n\n"
       << "Recommendations:\n";
    for (const auto& rec : r.recommendations) os << "  - " << rec << "\n";
    return os;
}