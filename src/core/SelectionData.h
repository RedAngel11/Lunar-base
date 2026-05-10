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

struct StructureParams {
    double wallThickness = 0.5;      // м
    double buriedFraction = 0.3;     // 0.0–1.0
    int numCompartments = 3;
    std::vector<double> compartmentVolumes = {50.0, 50.0, 50.0}; // м³
};

struct SelectionData {
    std::optional<LocationInfo> selectedLocation;
    std::optional<StructureType> selectedStructure;
    std::vector<MaterialType> selectedMaterials;
    StructureParams structureParams;

    void reset() {
        selectedLocation.reset(); selectedStructure.reset(); selectedMaterials.clear();
        structureParams = {};
    }
    [[nodiscard]] bool isReady() const {
        return selectedLocation.has_value() && selectedStructure.has_value() && !selectedMaterials.empty();
    }
};