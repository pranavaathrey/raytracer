#pragma once
#include "canvas.hpp"
#include "vector.hpp"

#define VIEWPORT_WIDTH 1.5
#define VIEWPORT_HEIGHT 1
#define VIEWPORT_DISTANCE 1

#define DRAW_DISTANCE 999999

// Camera
struct Camera {
    Vector position;
    Quaternion orientation;

    Camera(Vector coordinate, 
        float yaw, float pitch, float roll);
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
    float transparency;
    float specularity;
    float reflectivity;
    float refractiveIndex;
    Sphere(Vector c = Vector(0, 0, 0), float r = 0.0f, Colour hue = Colour(255, 255, 255), 
        float clearness = 1.0f, float specularExponent = 500.0f, float reflectiveness = 0.5f, float ior = 1.0f)
        : centre(c), radius(r), colour(hue), transparency(clearness),
          specularity(specularExponent), reflectivity(reflectiveness), refractiveIndex(ior) {}
};

// Functions
Vector canvasToViewport(int x, int y);

// lights
#define LIGHT_COUNT 1
extern float ambientLight;
extern pointLight pointLights[1];
extern directionalLight directionalLights[LIGHT_COUNT];

// objects
extern Sphere spheres[8];

void defineScene(pointLight *pointLights, directionalLight *directionalLights, 
                    Sphere *spheres);