#include "Canvas.hpp"

// Colour class definitions
    Colour::Colour() {
        red = 0; green = 0; blue = 0; alpha = 255;
    } // default constructor
    sf::Color Colour::toSFML() const {
        // converting custom colour definition to SFML's colors
        return sf::Color(red, green, blue, alpha);
    }
    // operator overloads
    Colour Colour::operator+(const Colour& other) const {
        return Colour(red + other.red, 
                    green + other.green, 
                    blue + other.blue);
    }
    Colour Colour::operator-(const Colour& other) const {
        return Colour(red - other.red, 
                    green - other.green, 
                    blue - other.blue);
    }
    Colour Colour::operator*(const Colour& other) const {
        return Colour(red * other.red / 255, 
                    green * other.green / 255, 
                    blue * other.blue / 255);
    }

// Canvas class definitions
    Canvas::Canvas(): pixels(WIDTH * HEIGHT) {}

    void Canvas::placePixel(const Colour& colour, int x, int y) {
        x = (WIDTH / 2) + x;
        y = (HEIGHT / 2) - y;

        if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){
            float alphaFactor = (float) colour.alpha/255;
            Colour newColour = Colour((pixels[y * WIDTH + x].red * (1 - alphaFactor)) 
                                            + (colour.red * alphaFactor),
                                    (pixels[y * WIDTH + x].green * (1 - alphaFactor)) 
                                            + (colour.green * alphaFactor),
                                    (pixels[y * WIDTH + x].blue * (1 - alphaFactor)) 
                                            + (colour.blue * alphaFactor));
            pixels[y * WIDTH + x] = newColour;
        }
    }
    void Canvas::draw(sf::RenderWindow& window) {
        sf::Image image;
        image.create(WIDTH, HEIGHT);

        for (int y = 0; y < HEIGHT; ++y)
            for (int x = 0; x < WIDTH; ++x)
                image.setPixel(x, y, pixels[y * WIDTH + x].toSFML());

        sf::Texture texture;
        texture.loadFromImage(image);
        sf::Sprite sprite(texture);
        window.draw(sprite);
    }
