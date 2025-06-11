#include "vector.hpp"
#include "canvas.hpp"
#include "scene.hpp"

#define INF 999999

void IntersectRaySphere();
Vector reflectRay(Vector ray, Vector normal);
float computeLightIntensity(Vector point, Vector normal, Vector viewVector, float specularExponent);
Colour traceRay(Vector camera, Vector D, float startDist, float endDist, int recursionDepth);