#include "scene.hpp"
#include "canvas.hpp"

Vector canvasToViewport(int x, int y) {
    return Vector((float) x * VIEWPORT_WIDTH/WIDTH, 
                  (float) y * VIEWPORT_HEIGHT/HEIGHT, 
                  VIEWPORT_DISTANCE);
}

Sphere spheres[3];

void defineScene(Sphere *spheres) {
    // Scene Objects
    spheres[0].centre = Vector(0, -1, 3);
    spheres[0].radius = 1;
    spheres[0].colour = Colour(255, 0, 0);  // Red

    spheres[1].centre = Vector(-2, 0, 4);
    spheres[1].radius = 1;
    spheres[1].colour = Colour(0, 255, 0);  // Green

    spheres[2].centre = Vector(2, 0, 4);
    spheres[2].radius = 1;
    spheres[2].colour = Colour(0, 0, 255);  // Blue
}