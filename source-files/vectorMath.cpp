#include "vector.hpp"

// Vector class definitions
    Vector Vector::operator+(Vector v2) {
        return Vector(x + v2.x, y + v2.y, z + v2.z);
    }
    Vector Vector::operator-(Vector v2) {
        return Vector(x - v2.x, y - v2.y, z - v2.z);
    }
    Vector Vector::operator*(float multiplier){
        return Vector(x * multiplier, y * multiplier, z * multiplier);
    }
    Vector Vector::operator/(float divisor){
        return Vector(x / divisor, y / divisor, z / divisor);
    }

// dot and cross products
float dot(Vector v1, Vector v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}
Vector cross(Vector v1, Vector v2) {
    // implement later
    return Vector(0, 0, 0);
}