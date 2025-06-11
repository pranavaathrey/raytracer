#pragma once
#include "canvas.hpp"
#include "vector.hpp"

#define VIEWPORT_WIDTH 1.5
#define VIEWPORT_HEIGHT 1
#define VIEWPORT_DISTANCE 1

#define DRAW_DISTANCE 999999

struct Orientation {
    float yaw;
    float pitch;
    float roll;
    Orientation(float y = 0.0f, float p = 0.0f, float r = 0.0f)
        : yaw(y), pitch(p), roll(r) {}
};

// Lights
struct pointLight {
    Colour colour;
    Vector position;
    float intensity;
    pointLight(Colour _colour = Colour(255, 255, 255), 
                Vector _position = Vector(0, 0, 0), 
                float _intensity = 1.0f)
        : colour(_colour), position(_position), intensity(_intensity) {}
};
struct directionalLight {
    Colour colour;
    Vector direction;
    float intensity;
    directionalLight(Colour _colour = Colour(255, 255, 255), 
                    Vector _direction = Vector(0, 0, 0), 
                    float _intensity = 1.0f)
        : colour(_colour), direction(_direction), intensity(_intensity) {}
};

// Shapes
struct Sphere {
    Vector centre;
    float radius;
    Colour colour;
    Sphere(Vector c = Vector(0, 0, 0), float r = 0.0f, Colour col = Colour(255, 255, 255))
        : centre(c), radius(r), colour(col) {}
};
struct Cylinder {
    Vector centre;
    float radius;
    float height;
    Colour colour;
    Cylinder(Vector c = Vector(0, 0, 0), float r = 0.0f, float h = 0.0f, Colour col = Colour(255, 255, 255))
        : centre(c), radius(r), height(h), colour(col) {}
};

// Functions
Vector canvasToViewport(int x, int y);

// lights
extern float ambientLight;
extern pointLight pointLights[1];
extern directionalLight directionalLights[1];

// objects
extern Sphere spheres[4];

void defineScene(pointLight *pointLights, directionalLight *directionalLights, 
                    Sphere *spheres);