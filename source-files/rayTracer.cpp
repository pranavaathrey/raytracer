#include "rays.hpp"
#include "BMP.hpp"

#define HDRI_WIDTH 4096
#define HDRI_HEIGHT 2048

// importing the HDRI (which is actually sdr but whatever)
std::vector<Colour> HDRI = loadFromBMP("input/HDRI.bmp", HDRI_WIDTH, HDRI_HEIGHT);

Colour getHDRIpixelFromRay(Vector ray) {
    // calculating uv coordinate
    float u = 0.5 + (atan2(ray.z, ray.x) / (2 * M_PI));
    float v = 0.5 - (asin(ray.y) / M_PI);
    // getting corresponding pixel coordinate in HDRI
    int i = int(u * HDRI_WIDTH);
    int j = int(v * HDRI_HEIGHT);
    i = i % HDRI_WIDTH;
    j = j % HDRI_HEIGHT;

    // returning colour at coordinate
    Colour backgroundColour = HDRI[j * HDRI_WIDTH + i];
    return backgroundColour;
}

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
    Vector reflectedRay = (normal * 2) * dot(normal, ray) - ray;
    return reflectedRay;
}

// checks if given ray gets refracted in its path, if yes, return true and refract the ray
bool refractRay(Vector incidentRay, Vector normal, float n1, float n2, Vector &refractedRay) {
    float cosTheta = -dot(incidentRay, normal);
    float eta = n1 / n2;
    float k = 1 - pow(eta, 2) * (1 - pow(cosTheta, 2));

    if (k < 0) {
        // this will be total internal reflection
        return false;
    } else {
        refractedRay = incidentRay * eta + normal * (eta * cosTheta - sqrt(k));
        return true;
    }
}

float fresnelSchlick(float cosTheta, float F0) {
    return F0 + (1 - F0) * powf(1 - cosTheta, 5);
}

struct infoSet {    
    float closest_t;    
    bool hitAnySphere;
    Sphere closestSphere;
};

infoSet closestIntersection(Vector cameraPoint, Vector ray, float startDist, float endDist) {
    infoSet set;
    
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
        infoSet set = closestIntersection(point, lightRay, 0.001f, 1);
        if(set.hitAnySphere)
            continue;

        float NdotL = dot(normal, lightRay);
        // Lambertian Diffused Light
        if (NdotL > 0) 
            intensity += light.intensity * (NdotL 
                       / (normal.magnitude * lightRay.magnitude));
        // Phong Specular Reflection
        if (specularExponent != -1) {
            Vector reflectionRay = (normal*2) * NdotL - lightRay;
            float RdotV = dot(reflectionRay, viewVector);

            if(RdotV > 0)
                intensity += light.intensity * pow(RdotV/
                            (reflectionRay.magnitude * viewVector.magnitude), specularExponent);
        }
    }
    for (directionalLight light: directionalLights) {
        lightRay = light.direction;

        // Check if point is in shadow (tmax is at infinity)
        infoSet set = closestIntersection(point, lightRay, 0.001f, INF);
        if(set.hitAnySphere)
            continue;

        float NdotL = dot(normal, lightRay);
        // Lambertian Diffused Light
        if (NdotL > 0) 
            intensity += light.intensity * (NdotL
                       / (normal.magnitude * lightRay.magnitude));
        // Phong Specular Reflection
        if (specularExponent != -1) {
            Vector reflectionRay = (normal*2) * NdotL - lightRay;
            float RdotV = dot(reflectionRay, viewVector);

            if(RdotV > 0)
                intensity += light.intensity * pow(RdotV/
                            (reflectionRay.magnitude * viewVector.magnitude), specularExponent);
        }
    }
    return intensity;
}

Colour traceRay(Vector cameraPoint, Vector ray, float startDist, float endDist, int recursionDepth) {
    infoSet set = closestIntersection(cameraPoint, ray, startDist, endDist);
    if (!set.hitAnySphere) {
        // return a background colour as no object was hit
        return getHDRIpixelFromRay(ray);
    }
    Vector sphereIntersectionPoint = cameraPoint + (ray * set.closest_t); 
    Vector sphereNormal = normalize(sphereIntersectionPoint - set.closestSphere.centre); 

    // compute local shaded colour
    Colour localColour = set.closestSphere.colour 
                * computeLightIntensity(sphereIntersectionPoint, sphereNormal,
                                        -ray, set.closestSphere.specularity);

    // return local colour once the recursion limit is reached
    if (recursionDepth <= 0)
        return localColour;
    
    // check for transparency & refractive index, get transmitted colour and blend
    if (set.closestSphere.refractiveIndex > 0.0f && set.closestSphere.transparency < 1.0f) {
        float n1 = 1.0f;  // air's refractive index
        float n2 = set.closestSphere.refractiveIndex;
        Vector N = sphereNormal;
        Vector viewRay = -ray;

        // flip the normal if the ray's coming from the inside
        if (dot(viewRay, N) < 0) {
            std::swap(n1, n2);
            N = -N;
        }
        // dealing with fresnel effects
        float cosTheta = dot(viewRay, N);
        float F0 = powf((n1 - n2) / (n1 + n2), 2.0f);
        float fresnel = fresnelSchlick(cosTheta, F0);

        Vector refractedRay;

        if (refractRay(ray, N, n1, n2, refractedRay)) {
            Colour refractedColour = traceRay(sphereIntersectionPoint + refractedRay * 0.01f,
                                            refractedRay, 0.001f, INF, recursionDepth - 1);
            Vector reflectedRay = reflectRay(ray, N);
            Colour reflectedColour = traceRay(sphereIntersectionPoint + reflectedRay * 0.01f,
                                            reflectedRay, 0.001f, INF, recursionDepth - 1);
            Colour resultingColour = reflectedColour * fresnel + refractedColour * (1 - fresnel);
            localColour = (localColour * set.closestSphere.transparency) 
                        + (resultingColour * (1.0f - set.closestSphere.transparency));
            
            // artistic: more reflections
            Vector _reflectedRay = reflectRay(-ray, sphereNormal);
            Colour _reflectedColour = traceRay(sphereIntersectionPoint, _reflectedRay, 0.05f, INF, recursionDepth - 1);
            
            localColour = localColour * (1 - 0.1) + (_reflectedColour * 0.1);
        } else {
            // total internal reflection
            Vector reflectedRay = reflectRay(ray, N);
            Colour reflectedColour = traceRay(sphereIntersectionPoint + reflectedRay * 0.01f,
                                            reflectedRay, 0.001f, INF, recursionDepth - 1);
            localColour = localColour * set.closestSphere.transparency
                        + reflectedColour * (1.0f - set.closestSphere.transparency);
        }
    }
    // check for transparency alone, no refraction
    else if (set.closestSphere.refractiveIndex == 0.0f && set.closestSphere.transparency < 1.0f) {
        Colour transmittedColour = traceRay(sphereIntersectionPoint, ray, 0.001f, INF, recursionDepth - 1);
        localColour = (localColour * set.closestSphere.transparency) 
                    + (transmittedColour * (1 - set.closestSphere.transparency));
    }
    
    // compute reflected colour if object is reflective
    if (set.closestSphere.reflectivity > 0) {
        float cosTheta = std::max(0.0f, dot(-ray, sphereNormal));
        float F0 = set.closestSphere.reflectivity;
        float fresnel = fresnelSchlick(cosTheta, F0);

        Vector reflectedRay = reflectRay(-ray, sphereNormal);
        Colour reflectedColour = traceRay(sphereIntersectionPoint + reflectedRay * 0.01f, 
                                            reflectedRay, 0.001f, INF, recursionDepth - 1);

        localColour = localColour * (1.0f - fresnel) + reflectedColour * fresnel;
    }
    return localColour;
}