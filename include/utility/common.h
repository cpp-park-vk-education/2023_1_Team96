#pragma once
#include <string>

enum class UnitType {
    Warrior, King,
};

enum class EnvType {
    Trees, Camp,
};

const std::string CONFIG = "../config.json";
const unsigned int CELL_TEXTURE_SIZE = 63;
const float SCALE = 1.5;
const unsigned int CELL_SIZE = CELL_TEXTURE_SIZE * SCALE;