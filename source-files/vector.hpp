#pragma once

class Vector {
    public:
    float x;
    float y;
    float z;

    Vector(float _x, float _y, float _z);

    Vector operator+(Vector v2);
    Vector operator-(Vector v2);
    float operator~();
};

float dot(Vector v1, Vector v2);
Vector cross(Vector v1, Vector v2);