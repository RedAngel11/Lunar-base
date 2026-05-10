#include "BaseCalculator.h"
#include <sstream>
#include <algorithm>

BaseCalculator::MaterialProps BaseCalculator::getMaterialProps(MaterialType type) {
    switch (type) {
        case MaterialType::RegolithConcrete: return {"Реголитовый бетон", 2200, 1.2, 0.6, 25.0, 0.9};
        case MaterialType::Aerogel:          return {"Аэрогель", 150, 0.02, 0.3, 15.0, 0.1};
        case MaterialType::TitaniumAlloy:    return {"Титановый сплав", 4500, 22.0, 0.9, 40.0, 0.0};
        case MaterialType::PolymerFoam:      return {"Полимерная пена", 300, 0.05, 0.4, 20.0, 0.3};
        default: throw std::invalid_argument("Unknown material type");
    }
}

CalculationReport BaseCalculator::calculate(const SelectionData& data) {
    if (!data.isReady()) throw std::runtime_error("Incomplete selection data");
    if (data.structureParams.wallThickness <= 0.0 || data.structureParams.numCompartments <= 0)
        throw std::invalid_argument("Invalid structure parameters");

    const auto& loc = *data.selectedLocation;
    const auto& mat = getMaterialProps(data.selectedMaterials.front());
    const auto& sp = data.structureParams;

    double totalVolume = 0.0;
    for (double v : sp.compartmentVolumes) totalVolume += v;

    // Радиационный фактор
    double solarFactor = 1.0 + (350.0 / 700.0) * 0.3;
    double effectiveRadiation = loc.radiationLevel * solarFactor;
    double regolithShield = 5.0 * sp.buriedFraction;
    double effShielding = sp.wallThickness * mat.radiationShielding + regolithShield;
    double radiationRisk = clamp(effectiveRadiation / (effShielding + 0.01), 0.0, 100.0);

    // Термический стресс
    double tempRange = loc.temperatureMax - loc.temperatureMin;
    double insulFactor = 1.0 / (mat.thermalConductivity + 0.01);
    double thermalStress = clamp(tempRange * (1.0 - insulFactor * 0.05) * (1.0 - sp.buriedFraction * 0.6), 0.0, 1000.0);

    // Срок службы
    double degradation = radiationRisk * 0.15 + thermalStress * 0.03;
    double lifespan = clamp(mat.durability / (1.0 + degradation), 0.0, 50.0);

    // Безопасность
    double rScore = std::max(0.0, 100.0 - radiationRisk * 40.0);
    double tScore = std::max(0.0, 100.0 - thermalStress * 0.2);
    double sScore = std::min(100.0, 50.0 + sp.wallThickness * 10.0 + sp.buriedFraction * 30.0);
    double safety = rScore * 0.4 + tScore * 0.3 + sScore * 0.3;

    // Стоимость
    auto costRes = CostCalculator::calculate(totalVolume, sp.wallThickness, mat.density, mat.inSituProducibility);
    double complexityPenalty = sp.numCompartments > CostCalculator::BASE_COMPARTMENTS ?
        (sp.numCompartments - CostCalculator::BASE_COMPARTMENTS) * CostCalculator::COMPLEXITY_PENALTY : 0.0;
    costRes.totalCost += complexityPenalty;

    // Рекомендации
    std::vector<std::string> recs;
    if (lifespan < 10.0) recs.push_back("Срок службы мал. Увеличьте толщину стенок или добавьте реголит.");
    if (radiationRisk > 5.0) recs.push_back("Высокий радиационный риск. Рекомендуется закапывание или аэрогель.");
    if (thermalStress > 200.0) recs.push_back("Сильный термический стресс. Увеличьте долю изоляционных материалов.");
    if (recs.empty()) recs.push_back("Конфигурация оптимальна для выбранных условий.");

    return CalculationReport{
        loc.name, mat.name, sp.wallThickness, sp.buriedFraction, totalVolume,
        costRes.totalCost, costRes.earthLaunchMass, costRes.localMass,
        lifespan, safety, radiationRisk, thermalStress, std::move(recs)
    };
}

std::ostream& operator<<(std::ostream& os, const CalculationReport& r) {
    os << "📊 ОТЧЁТ ПО БАЗЕ\n"
       << "━━━━━━━━━━━━━━━━━━━━━━\n"
       << "🌍 Локация: " << r.locationName << "\n"
       << "🧱 Материал: " << r.materialName << ", стенка " << r.wallThickness << " м\n"
       << "🏗️ Объём: " << r.totalVolume << " м³, закапывание: " << (r.buriedFraction*100) << "%\n\n"
       << "💰 Стоимость: $" << (r.totalCost/1e9) << " млрд\n"
       << "   • Доставка: " << r.earthLaunchMass << " кг × $150 000\n"
       << "   • Строительство на Луне: " << r.localMass << " кг × $500\n\n"
       << "⏳ Срок службы: " << r.estimatedLifespan << " лет\n"
       << "   • Радиационный риск: " << r.radiationRisk << "\n"
       << "   • Термический стресс: " << r.thermalStress << "\n\n"
       << "☢️ Безопасность: " << static_cast<int>(r.safetyScore) << "/100\n\n"
       << "💡 Рекомендации:\n";
    for (const auto& rec : r.recommendations) os << "  • " << rec << "\n";
    return os;
}