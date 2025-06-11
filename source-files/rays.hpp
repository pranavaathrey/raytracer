#include "vector.hpp"
#include "canvas.hpp"
#include "scene.hpp"

#define INF 999999

void IntersectRaySphere();

float computeLightIntensity(Vector point, Vector normal);
Colour traceRay(Vector camera, Vector D, float startDist, float endDist);