#include <iostream>
#include <iomanip>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "canvas.hpp"
#include "scene.hpp"
#include "rays.hpp"
#include "postProcess.hpp"
#include "BMP.hpp"

int main() {
    // initialize canvas & its window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Canvas");
    int pixelCount = HEIGHT * WIDTH, lastProgress = -1;    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Render resolution: " << WIDTH << "x" << HEIGHT << std::endl
              << "Starting render..." << std::endl;
    Canvas canvas;

    // place the camera and define the scene (see sceneDefinition.cpp)
    // camera's arguments: position, yaw, pitch and roll angles (anti-clockwise), in radians
    Camera camera = Camera(Vector(0, 1, -6), 2, 5, 2.5);
    defineScene(pointLights, directionalLights, spheres);

    // start timer
    auto startTime = std::chrono::high_resolution_clock::now();

    // render loop
    for (int y = -HEIGHT/2; y < HEIGHT/2; ++y) {
        for (int x = -WIDTH/2; x < WIDTH/2; ++x) {
            // determine which point on the viewport corresponds to this pixel
            // (AKA our normalized light ray), also rotate it to go along the camera's orientation
            Vector ray = camera.orientation.rotate(canvasToViewport(x, y));

            // determine the colour seen through that square & paint the pixel with that colour
            // also define number of recursive reflection calls
            Colour seenColour = traceRay(camera.position, ray, 1, DRAW_DISTANCE, 4);
            canvas.placePixel(seenColour, x, y);
        }
        // calculate and print progress so far 
        // (flushing is ridiculously expensive, don't try to "improve" it)
        int currentProgress = (((float)y + (float)HEIGHT/2) / (float)HEIGHT) * 100;
        if(currentProgress != lastProgress) {
            std::cout << "\r" << currentProgress << "% complete." << std::flush;
            lastProgress = currentProgress;
        }
    }
    // end timer and remark in console
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "\rRender complete. Time taken: " << duration / 1000.0f << " seconds." << std::endl;

    // doing a post processing pass that removes the bright artifacts seen in the distance
    std::cout << "\rPost processing...";
    eliminateDistantReflectionAcne(canvas.pixels);
    std::cout << " complete." << std::endl;    

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