#pragma once
#include <string>
#include <optional>
#include <vector>

enum class LocationType { None, MareSerenitatis, ShackletonCrater, Highlands, OceanOfStorms, TychoCrater };
enum class StructureType { None, SealedModule, UndergroundBunker, InflatableDome, RegolithPrinted };
enum class MaterialType { None, RegolithConcrete, Aerogel, TitaniumAlloy, PolymerFoam };

struct LocationInfo {
    std::string name;
    double radiationLevel;
    double temperatureMin, temperatureMax;
    double groundStability;
    float relX, relY;
};

struct CustomMaterialParams {
    bool isActive = false;
    double density = 1000.0;
    double thermalConductivity = 1.0;
    double radiationShielding = 0.5;
    double durability = 20.0;
    double inSituProducibility = 0.5;
    double devTimeYears = 0.0;
};

struct StructureParams {
    StructureType type = StructureType::None;
    double wallThickness = 0.5;
    double buriedFraction = 0.3;
    int numCompartments = 3;
    std::vector<double> compartmentVolumes = {50.0, 50.0, 50.0};
};

struct SelectionData {
    std::optional<LocationInfo> selectedLocation;
    std::optional<StructureType> selectedStructure;
    std::vector<MaterialType> selectedMaterials;
    StructureParams structureParams;
    CustomMaterialParams customMat;

    void reset() {
        selectedLocation.reset(); selectedStructure.reset(); selectedMaterials.clear();
        structureParams = {}; customMat = {};
    }
    [[nodiscard]] bool isReady() const {
        return selectedLocation.has_value() && selectedStructure.has_value() && !selectedMaterials.empty();
    }
};