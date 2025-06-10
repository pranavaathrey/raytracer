#include "vector.hpp"
#include "canvas.hpp"
#include "scene.hpp"

#define INF 999999

void IntersectRaySphere();

Colour traceRay(Vector camera, Vector D, float startDist, float endDist);