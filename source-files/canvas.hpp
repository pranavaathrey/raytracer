#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>
#include <algorithm>

#define WIDTH (1080*1)
#define HEIGHT (720*1)

// stores 32-bit colour. (8 bit red, green, blue and alpha channels)
class Colour {
    public:
    uint8_t red, green, blue, alpha;

    Colour();  // default constructor
    template<typename T>
    Colour(T r, T g, T b, T a = 255);  // template declaration

    sf::Color toSFML() const;

    Colour operator+(const Colour&) const;
    Colour operator-(const Colour&) const;
    Colour operator*(const Colour&) const;
    Colour operator*(const float&) const;

    friend class Canvas;
};
template<typename T> // template definition
Colour::Colour(T r, T g, T b, T a) {
    red = std::clamp<T>(r, 0, 255);
    green = std::clamp<T>(g, 0, 255);
    blue = std::clamp<T>(b, 0, 255);
    alpha = std::clamp<T>(a, 0, 255);
} 

// stores the pixel buffer (the image itself)
class Canvas { 
    public:
    std::vector<Colour> pixels;

    Canvas();
    void placePixel(const Colour& c, int x, int y);
    void draw(sf::RenderWindow& window);
};
