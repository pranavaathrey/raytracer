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
    Vector Vector::operator-() {
        return Vector(-x, -y, -z);
    }

// other basic vector operations
Vector normalize(Vector vector) {
    return vector / vector.magnitude;
}
float dot(Vector v1, Vector v2) {
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}
Vector cross(Vector v1, Vector v2) {
    // implement later
    return Vector(0, 0, 0);
}

// Quaternion class definitions
    Quaternion::Quaternion(float angle, Vector axis) {
        // normalize axis and convert angle to degrees
        axis = normalize(axis);
        float halfAngle = (angle * M_PI/180) / 2;
        
        w = cos(halfAngle);
        float s = sin(halfAngle);

        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
    }

    Vector Quaternion::rotate(const Vector& vector) const {
        Quaternion vectorQuat(0, vector.x, vector.y, vector.z);
        Quaternion result = (*this) * vectorQuat * this->conjugate();
        return Vector(result.x, result.y, result.z);
    }

    Quaternion Quaternion::conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }

    Quaternion Quaternion::operator*(const Quaternion& q) const {
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        );
    }