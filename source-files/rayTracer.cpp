#include "rays.hpp"

struct Solution {
    float t1, t2;
};

Solution IntersectRaySphere(Vector O, Vector D, Sphere sphere) {
    // finds the two points where the ray intersects a sphere (if any) using the quadratic equation
    // O = camera's position, D = position on the viewport plane that corresponds to the pixel on the canvas

    float r = sphere.radius;
    Vector vectorO = Vector(O.x, O.y, O.z);
    Vector CO = vectorO - sphere.centre;

    float a = dot(D, D);
    float b = 2*dot(CO, D);
    float c = dot(CO, CO) - r*r;

    Solution soln;
    soln.t1 = INF;
    soln.t2 = INF;

    float discriminant = b*b - 4*a*c;
    if (discriminant < 0)
        return soln;

    soln.t1 = (-b + sqrt(discriminant)) / (2*a);
    soln.t2 = (-b - sqrt(discriminant)) / (2*a);
    return soln;
}


float computeLightIntensity(Vector point, Vector normal) {
    float intensity = ambientLight;
    Vector lightRay;

    for (pointLight light: pointLights) {
        lightRay = light.position - point;

        // The Lambertian Diffused Light Equation
        if (dot(normal, lightRay) > 0) 
            intensity += light.intensity * (dot(normal, lightRay) / (normal.magnitude * lightRay.magnitude));
    }
    for (directionalLight light: directionalLights) {
        lightRay = light.direction;

        // The Lambertian Diffused Light Equation
        if (dot(normal, lightRay) > 0) 
            intensity += light.intensity * (dot(normal, lightRay) / (normal.magnitude * lightRay.magnitude));
    }

    return intensity;
}

Colour traceRay(Vector cameraPoint, Vector viewportPoint, float startDist, float endDist) {
    float closest_t = INF;
    bool hitAnySphere = false;
    Sphere closestSphere = spheres[0];

    for (Sphere sphere: spheres) {
        Solution soln = IntersectRaySphere(cameraPoint, viewportPoint, sphere);

        if ((soln.t1 > startDist && soln.t1 < endDist) && soln.t1 < closest_t) {
            closest_t = soln.t1;

            hitAnySphere = true;
            closestSphere = sphere;
        }
        if ((soln.t2 > startDist && soln.t2 < endDist) && soln.t2 < closest_t) {
            closest_t = soln.t2;
            
            hitAnySphere = true;
            closestSphere = sphere;
        }
    }

    Vector sphereIntersectionPoint = cameraPoint + (viewportPoint * closest_t); 
    Vector sphereNormal = (sphereIntersectionPoint - closestSphere.centre) 
                        / (sphereIntersectionPoint - closestSphere.centre).magnitude; 

    if (!hitAnySphere) {
        // return a transparent colour as no object was hit
        return Colour(0, 0, 0, 0);
    }
    return closestSphere.colour * computeLightIntensity(sphereIntersectionPoint, sphereNormal);
}