#include "vector.hpp"
#include <cmath>

// Vector class definitions
    Vector::Vector(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    Vector Vector::operator+(Vector v2) {
        return Vector(x + v2.x, y + v2.y, z + v2.z);
    }
    Vector Vector::operator-(Vector v2) {
        return Vector(x - v2.x, y - v2.y, z - v2.z);
    }
    float Vector::operator~() {
        return sqrt((x*x)+(y*y)+(z*z));
    }

// dot and cross products
float dot(Vector v1, Vector v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}
Vector cross(Vector v1, Vector v2) {
    // implement later
    return Vector(0, 0, 0);
}