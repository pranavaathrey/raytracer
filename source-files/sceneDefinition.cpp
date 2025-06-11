#include "scene.hpp"
#include "canvas.hpp"

Vector canvasToViewport(int x, int y) {
    return Vector((float) x * VIEWPORT_WIDTH/WIDTH, 
                  (float) y * VIEWPORT_HEIGHT/HEIGHT, 
                  VIEWPORT_DISTANCE);
}

// Scene Lights
float ambientLight = 0.2f;
pointLight pointLights[1];
directionalLight directionalLights[LIGHT_COUNT];

// Scene Objects
Sphere spheres[4];

void defineScene(pointLight *pointLights, directionalLight *directionalLights, 
                    Sphere *spheres) {
    // Scene Lights (note: colours not fully implemented)
    pointLights[0].colour = Colour(255, 255, 255);
    pointLights[0].position = Vector(2, 1, 0);
    pointLights[0].intensity = 0.6f;

    for (int i = 0; i < LIGHT_COUNT; i++) {
        directionalLights[i].colour = Colour(255, 255, 255);
        directionalLights[i].direction = Vector(1 + i/4, 4 + i/2, 4 + i/2);
        directionalLights[i].intensity = (float) 0.2f / (LIGHT_COUNT);
    }

    // Scene Objects
    spheres[0].centre = Vector(0, -1, 3);
    spheres[0].radius = 1;
    spheres[0].colour = Colour(255, 0, 0);  // Red sphere
    spheres[0].specularity = 500.0f;
    spheres[0].reflectivity = 0.2f;

    spheres[1].centre = Vector(-2, 0, 4);
    spheres[1].radius = 1;
    spheres[1].colour = Colour(0, 255, 0);  // Green sphere
    spheres[1].specularity = 10.0f;
    spheres[1].reflectivity = 0.3f;

    spheres[2].centre = Vector(2, 0, 4);
    spheres[2].radius = 1;
    spheres[2].colour = Colour(0, 0, 255);  // Blue sphere
    spheres[2].specularity = 100.0f;
    spheres[2].reflectivity = 0.4f;

    spheres[3].centre = Vector(0, -5001, 0);
    spheres[3].radius = 5000;
    spheres[3].colour = Colour(255, 255, 0);  // Big yellow sphere
    spheres[3].specularity = 1000.0f;
    spheres[3].reflectivity = 0.5f;
}