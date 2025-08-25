#include "vector.hpp"
#include "canvas.hpp"
#include "scene.hpp"

#define INF 999999

void IntersectRaySphere();

Vector reflectRay(Vector ray, Vector normal);
bool refractRay(Vector incidentRay, Vector normal, float n1, float n2, Vector &refractedRay);

float fresnelSchlick(float cosTheta, float F0);
float computeLightIntensity(Vector point, Vector normal, Vector viewVector, float specularExponent);

Colour traceRay(Vector cameraPoint, Vector ray, float startDist, float endDist, int recursionDepth);