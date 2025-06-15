#include "scene.hpp"
#include "canvas.hpp"

Vector canvasToViewport(int x, int y) {
    return Vector((float) x * VIEWPORT_WIDTH/WIDTH, 
                  (float) y * VIEWPORT_HEIGHT/HEIGHT, 
                  VIEWPORT_DISTANCE);
}

// Camera struct definition
    Camera::Camera(Vector coordinate, 
                float yaw, float pitch, float roll) {
        position = coordinate;

        Quaternion yawQuat(yaw, Vector(0, 1, 0));
        Quaternion pitchQuat(pitch, Vector(1, 0, 0));
        Quaternion rollQuat(roll, Vector(0, 0, 1));
        
        // order matters. here, we rotate by roll, then pitch, then yaw.
        orientation = yawQuat * pitchQuat * rollQuat;
    }

// Scene Lights
float ambientLight = 0.2f;
pointLight pointLights[1];
directionalLight directionalLights[LIGHT_COUNT];

// Scene Objects
Sphere spheres[8];

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
    spheres[0].transparency = 0.95f;
    spheres[0].specularity = 500.0f;
    spheres[0].reflectivity = 0.2f;
    spheres[0].refractiveIndex = 0.5f;

    spheres[1].centre = Vector(-2, 0, 4);
    spheres[1].radius = 1;
    spheres[1].colour = Colour(0, 255, 0);  // Green sphere
    spheres[1].transparency = 0.75f;
    spheres[1].specularity = 10.0f;
    spheres[1].reflectivity = 0.3f;
    spheres[1].refractiveIndex = 2.5f;

    spheres[2].centre = Vector(2, 0, 4);
    spheres[2].radius = 1;
    spheres[2].colour = Colour(0, 0, 255);  // Blue sphere
    spheres[2].transparency = 0.5f;
    spheres[2].specularity = 100.0f;
    spheres[2].reflectivity = 0.4f;
    spheres[2].refractiveIndex = 1.5f;

    spheres[3].centre = Vector(0, -5001, 0);
    spheres[3].radius = 5000;
    spheres[3].colour = Colour(255, 255, 0);  // Big yellow sphere
    spheres[3].transparency = 1.0f;
    spheres[3].specularity = 1000.0f;
    spheres[3].reflectivity = 0.5f;
    spheres[3].refractiveIndex = 7.5f;

    spheres[4].centre = Vector(0, 1, -1);
    spheres[4].radius = 1.4;
    spheres[4].colour = Colour(255, 255, 255);  // White sphere
    spheres[4].transparency = 0.01f;
    spheres[4].specularity = 1000.0f;
    spheres[4].reflectivity = 0.35f;
    spheres[4].refractiveIndex = 1.5f;

    // in front of refraction showcase
    spheres[5].centre = Vector(0, -1, -3);
    spheres[5].radius = 1;
    spheres[5].colour = Colour(255, 0, 0);  // Red sphere
    spheres[5].transparency = 0.95f;
    spheres[5].specularity = 500.0f;
    spheres[5].reflectivity = 0.2f;
    spheres[5].refractiveIndex = 0.5f;

    spheres[6].centre = Vector(-2, 0, -4);
    spheres[6].radius = 1;
    spheres[6].colour = Colour(0, 255, 0);  // Green sphere
    spheres[6].transparency = 0.75f;
    spheres[6].specularity = 10.0f;
    spheres[6].reflectivity = 0.3f;
    spheres[6].refractiveIndex = 2.5f;

    spheres[7].centre = Vector(2, 0, -4);
    spheres[7].radius = 1;
    spheres[7].colour = Colour(0, 0, 255);  // Blue sphere
    spheres[7].transparency = 0.5f;
    spheres[7].specularity = 100.0f;
    spheres[7].reflectivity = 0.4f;
    spheres[7].refractiveIndex = 1.5f;
}