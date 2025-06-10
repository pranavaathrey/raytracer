#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>
#include <algorithm>

#define WIDTH 720
#define HEIGHT 720

class Colour { // Colour class declaration
    private:
    uint8_t red, green, blue, alpha;

    public:
    Colour();  // default constructor
    template<typename T>
    Colour(T r, T g, T b, T a = 255);  // template declaration

    sf::Color toSFML() const;

    Colour operator+(const Colour&) const;
    Colour operator-(const Colour&) const;
    Colour operator*(const Colour&) const;

    friend class Canvas;
};
template<typename T> // template definition
Colour::Colour(T r, T g, T b, T a) {
    red = std::clamp<T>(r, 0, 255);
    green = std::clamp<T>(g, 0, 255);
    blue = std::clamp<T>(b, 0, 255);
    alpha = std::clamp<T>(a, 0, 255);
} 

class Canvas { // Canvas class declaration
    private:
    std::vector<Colour> pixels;

    public:
    Canvas();
    void placePixel(const Colour& c, int x, int y);
    void draw(sf::RenderWindow& window);
};
