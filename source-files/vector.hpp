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

class Quaternion {
    public:
    float w, x, y, z;

    Quaternion(float angle = 0.0f, Vector axis = Vector(0, 1, 0));

    Vector rotate(const Vector& vector) const;
    // inverse to keep the quaternion non crazy
    Quaternion conjugate() const;

    Quaternion operator*(const Quaternion& q) const;

    // non-angle constructor (might come in handy sometimes)
    private:
    Quaternion(float w_, float x_, float y_, float z_)
        : w(w_), x(x_), y(y_), z(z_) {}
};


float dot(Vector v1, Vector v2);
Vector cross(Vector v1, Vector v2);