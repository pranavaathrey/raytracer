#include "postProcess.hpp"

struct boxKernel3x3 {
    Colour c1, c2, c3;
    Colour c4, c5, c6;
    Colour c7, c8, c9;
};

// helper functions
int getBrightness(Colour colour) {
    return ((float)colour.red * 0.299
          + (float)colour.green * 0.587
          + (float)colour.blue * 0.114); 
    // integer value for perceived brightness
}

boxKernel3x3 getNeighbours(std::vector<Colour> &pixels, int x, int y) {
    boxKernel3x3 neighbourhood;

    neighbourhood.c1 = pixels[(y - 1)*WIDTH + (x - 1)];
    neighbourhood.c2 = pixels[(y - 1)*WIDTH + (x    )];
    neighbourhood.c3 = pixels[(y - 1)*WIDTH + (x + 1)];

    neighbourhood.c4 = pixels[(y    )*WIDTH + (x - 1)];
    neighbourhood.c5 = pixels[(y    )*WIDTH + (x    )];
    neighbourhood.c6 = pixels[(y    )*WIDTH + (x + 1)];

    neighbourhood.c7 = pixels[(y + 1)*WIDTH + (x - 1)];
    neighbourhood.c8 = pixels[(y + 1)*WIDTH + (x    )];
    neighbourhood.c9 = pixels[(y + 1)*WIDTH + (x + 1)];

    return neighbourhood;
}

int kernelAvgBrightnessNC(boxKernel3x3 kernel) {
    // NC = center pixel is ignored
    return
    ( getBrightness(kernel.c1) + getBrightness(kernel.c2) + getBrightness(kernel.c3) +
      getBrightness(kernel.c4) +                            getBrightness(kernel.c6) +
      getBrightness(kernel.c7) + getBrightness(kernel.c8) + getBrightness(kernel.c9) )
    / 8;
}
Colour kernelAvgColourNC(boxKernel3x3 kernel) {
    return Colour(
        ( kernel.c1.red + kernel.c2.red + kernel.c3.red +
          kernel.c4.red +                 kernel.c6.red +
          kernel.c7.red + kernel.c8.red + kernel.c9.red ) / 8,

        ( kernel.c1.green + kernel.c2.green + kernel.c3.green +
          kernel.c4.green +                   kernel.c6.green +
          kernel.c7.green + kernel.c8.green + kernel.c9.green ) / 8,

        ( kernel.c1.blue + kernel.c2.blue + kernel.c3.blue +
          kernel.c4.blue +                  kernel.c6.blue +
          kernel.c7.blue + kernel.c8.blue + kernel.c9.blue ) / 8
    );
}

// doing a pass that removes the bright artifacts seen in the distance
void eliminateDistantReflectionAcne(std::vector<Colour> &pixels) {
    for (int y = 1; y < HEIGHT-1; y++) {
        for (int x = 1; x < WIDTH-1; x++) {
            boxKernel3x3 neighbourhood = getNeighbours(pixels, x, y);
            if (getBrightness(pixels[(y)*WIDTH + (x)]) 
                        > kernelAvgBrightnessNC(neighbourhood)) {
                pixels[(y)*WIDTH + (x)] 
                    = kernelAvgColourNC(neighbourhood);
            }
        }
    }
}