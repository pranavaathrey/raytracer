#pragma once
#include "canvas.hpp"
#include "vector.hpp"

#define VIEWPORT_WIDTH 1
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

Vector canvasToViewport(int x, int y);

extern Sphere spheres[3];
void defineScene(Sphere *spheres);