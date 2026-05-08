// src/core/SelectionData.hpp
#pragma once
#include <string>
#include <optional>
#include <vector>

enum class LocationType { None, MareSerenitatis, ShackletonCrater, Highlands };
enum class StructureType { None, SealedModule, UndergroundBunker, InflatableDome, RegolithPrinted };
enum class MaterialType { None, RegolithConcrete, Aerogel, TitaniumAlloy, PolymerFoam };

struct LocationInfo {
    std::string name;
    double radiationLevel;
    double temperatureMin;
    double temperatureMax;
    double groundStability;
    int hotspotX, hotspotY;
};

struct SelectionData {
    std::optional<LocationInfo> selectedLocation;
    std::optional<StructureType> selectedStructure;
    std::vector<MaterialType> selectedMaterials;

    void reset() {
        selectedLocation.reset();
        selectedStructure.reset();
        selectedMaterials.clear();
    }

    [[nodiscard]] bool isReady() const {
        return selectedLocation.has_value() &&
               selectedStructure.has_value() &&
               !selectedMaterials.empty();
    }
};