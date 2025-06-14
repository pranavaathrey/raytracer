#include <iostream>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "canvas.hpp"
#include "scene.hpp"
#include "rays.hpp"
#include "BMP.hpp"

int main() {
    std::cout << "Working..." << std::endl;

    // initialize canvas & its window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Canvas");
    Canvas canvas;

    //place the camera and define the scene (see sceneDefinition.cpp)
    Vector cameraCoordinates(0, 1, -6);
    Vector cameraOrientation(0, 0, 0);
    defineScene(pointLights, directionalLights, spheres);

    // start timer
    auto startTime = std::chrono::high_resolution_clock::now();

    // render loop
    for (int y = -HEIGHT/2; y < HEIGHT/2; ++y) {
        for (int x = -WIDTH/2; x < WIDTH/2; ++x) {
            // determine which point on the viewport corresponds to this pixel
            // (AKA our normalized light ray)
            Vector ray = canvasToViewport(x, y);

            // determine the colour seen through that square & paint the pixel with that colour
            // also define number of recursive reflection calls
            Colour seenColour = traceRay(cameraCoordinates, ray, 1, DRAW_DISTANCE, 4);
            canvas.placePixel(seenColour, x, y);
        }
    }
    // end timer and remark in console
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "\rRender complete. Time taken: " << duration << " ms." << std::endl;

    // saving render to a bitmap file
    saveAsBMP(canvas.pixels, WIDTH, HEIGHT, "output.bmp"); 
    
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