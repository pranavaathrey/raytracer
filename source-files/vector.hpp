#pragma once
#include <cmath>

class Vector {
    public:
    float x, y, z;
    float magnitude;

    Vector(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
            : x(_x), y(_y), z(_z) {
        magnitude = sqrt((x*x)+(y*y)+(z*z));
    }

    Vector operator+(Vector v2);
    Vector operator-(Vector v2);
    Vector operator*(float multiplier);
    Vector operator/(float divisor);
    Vector operator-();
};

float dot(Vector v1, Vector v2);
Vector cross(Vector v1, Vector v2);