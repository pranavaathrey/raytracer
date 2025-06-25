#pragma once
#include <string>
#include <cstdint>
#include "canvas.hpp"

void saveAsBMP(const std::vector<Colour>& pixels, 
                int width, 
                int height, 
                const std::string& filename);

std::vector<Colour> loadFromBMP(const std::string& filename, 
                int width, int height);