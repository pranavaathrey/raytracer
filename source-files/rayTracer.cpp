#include "rays.hpp"

struct Solution {
    float t1, t2;
};

Solution IntersectRaySphere(Vector O, Vector D, Sphere sphere) {
    // finds the two points where the ray intersects a sphere (if any) using the quadratic equation
    // O = camera's position, D = light ray emerging from viewport

    float r = sphere.radius;
    Vector CO = O - sphere.centre;

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

Vector reflectRay(Vector ray, Vector normal) {
    Vector newVector = (normal * 2) * dot(normal, ray) - ray;
    return newVector;
}

struct Set {    
    float closest_t;    
    bool hitAnySphere;
    Sphere closestSphere;
};

Set closestIntersection(Vector cameraPoint, Vector ray, float startDist, float endDist) {
    Set set;
    
    set.closest_t = INF;
    set.hitAnySphere = false;
    set.closestSphere = spheres[0];

    for (Sphere sphere: spheres) {
        Solution soln = IntersectRaySphere(cameraPoint, ray, sphere);

        if ((soln.t1 > startDist && soln.t1 < endDist) && soln.t1 < set.closest_t) {
            set.closest_t = soln.t1;

            set.hitAnySphere = true;
            set.closestSphere = sphere;
        }
        if ((soln.t2 > startDist && soln.t2 < endDist) && soln.t2 < set.closest_t) {
            set.closest_t = soln.t2;
            
            set.hitAnySphere = true;
            set.closestSphere = sphere;
        }
    }
    return set;
}

float computeLightIntensity(Vector point, Vector normal, Vector viewVector, float specularExponent) {
    // determines how lit a pixel should be according to the environment around it
    float intensity = ambientLight;
    Vector lightRay;

    for (pointLight light: pointLights) {
        lightRay = light.position - point;

        // Check if point is in shadow (tmax is at the light's pos, 1)
        Set set = closestIntersection(point, lightRay, 0.05f, 1);
        if(set.hitAnySphere)
            continue;

        // From the Lambertian Diffused Light Equation
        if (dot(normal, lightRay) > 0) 
            intensity += light.intensity * (dot(normal, lightRay) 
                       / (normal.magnitude * lightRay.magnitude));
        // From the Phong Specular Reflection Equation
        if (specularExponent != -1) {
            Vector reflectionRay = (normal*2) * dot(normal, lightRay) - lightRay;
            float RdotV = dot(reflectionRay, viewVector);

            if(RdotV > 0)
                intensity += light.intensity 
                           * pow(RdotV/(reflectionRay.magnitude * viewVector.magnitude), specularExponent);
        }
    }
    for (directionalLight light: directionalLights) {
        lightRay = light.direction;

        // Check if point is in shadow (tmax is at infinity)
        Set set = closestIntersection(point, lightRay, 0.05f, INF);
        if(set.hitAnySphere)
            continue;

        // From the Lambertian Diffused Light Equation
        if (dot(normal, lightRay) > 0) 
            intensity += light.intensity * (dot(normal, lightRay) 
                       / (normal.magnitude * lightRay.magnitude));
        // From the Phong Specular Reflection Equation
        if (specularExponent != -1) {
            Vector reflectionRay = (normal*2) * dot(normal, lightRay) - lightRay;
            float RdotV = dot(reflectionRay, viewVector);

            if(RdotV > 0)
                intensity += light.intensity
                           * pow(RdotV/(reflectionRay.magnitude * viewVector.magnitude), specularExponent);
        }
    }
    return intensity;
}

Colour traceRay(Vector cameraPoint, Vector ray, float startDist, float endDist, int recursionDepth) {
    Set set = closestIntersection(cameraPoint, ray, startDist, endDist);
    if (!set.hitAnySphere) {
        // return a transparent colour as no object was hit
        return Colour(0, 0, 0, 0);
    }
    Vector sphereIntersectionPoint = cameraPoint + (ray * set.closest_t); 
    Vector sphereNormal = (sphereIntersectionPoint - set.closestSphere.centre) 
                        / (sphereIntersectionPoint - set.closestSphere.centre).magnitude; 

    Colour localColour = set.closestSphere.colour 
                * computeLightIntensity(sphereIntersectionPoint, sphereNormal,
                                        -ray, set.closestSphere.specularity);

    // return local colour once the recursion limit is reached / object isnt reflective
    if (recursionDepth <= 0 || set.closestSphere.reflectivity <= 0)
        return localColour;

    // otherwise, compute reflected colour
    Vector reflectedRay = reflectRay(-ray, sphereNormal);
    Colour reflectedColour = traceRay(sphereIntersectionPoint, reflectedRay, 0.05f, INF, recursionDepth - 1);
    
    // returns a blend between reflected and local colours according to reflectivity of sphere
    return (localColour * (1 - set.closestSphere.reflectivity))
         + (reflectedColour * set.closestSphere.reflectivity);
}