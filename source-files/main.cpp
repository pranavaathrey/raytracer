#include <iostream>
#include <SFML/Graphics.hpp>

#include "canvas.hpp"
#include "scene.hpp"
#include "rays.hpp"

int main() {
    // initialize canvas & its window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Canvas");
    Canvas canvas;

    //place the camera and define the scene (see sceneDefinition.cpp)
    Vector cameraCoordinates(0, 0, 0);
    Orientation cameraOrientation(0, 0, 0);
    defineScene(pointLights, directionalLights, spheres);

    // render loop
    for (int y = -HEIGHT/2; y < HEIGHT/2; ++y) {
        for (int x = -WIDTH/2; x < WIDTH/2; ++x) {
            // colouring in the background with a gradient
            float t = ((y + HEIGHT/2.0f) / HEIGHT) + 0.5;
            canvas.placePixel(Colour(85, 85, 85) * t, x, y);

            // determine which point on the viewport corresponds to this pixel
            Vector viewportPoint = canvasToViewport(x, y);

            // determine the colour seen through that square & paint the pixel with that colour
            Colour seenColour = traceRay(cameraCoordinates, viewportPoint, 1, DRAW_DISTANCE);
            canvas.placePixel(seenColour, x, y);
        }
    }

    std::cout << "Render complete." << std::endl;
    
    // displaying the render in a window
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();
        canvas.draw(window);
        window.display();
    }
    return 0;
}