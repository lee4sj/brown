#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stdlib.h>
#include <iostream>

#define M_2PI (2 * M_PI)

inline float min(float a, float b) { return a < b ? a : b; }
inline float max(float a, float b) { return a > b ? a : b; }

class Vector3
{
public:
    union
    {
        struct { float x, y, z; };
        float xyz[3];
    };

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vector3(const Vector3 &vec) : x(vec.x), y(vec.y), z(vec.z) {}

    Vector3 operator - () const { return Vector3(-x, -y, -z); }

    Vector3 operator + (const Vector3 &vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
    Vector3 operator - (const Vector3 &vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
    Vector3 operator * (const Vector3 &vec) const { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
    Vector3 operator / (const Vector3 &vec) const { return Vector3(x / vec.x, y / vec.y, z / vec.z); }
    Vector3 operator + (float s) const { return Vector3(x + s, y + s, z + s); }
    Vector3 operator - (float s) const { return Vector3(x - s, y - s, z - s); }
    Vector3 operator * (float s) const { return Vector3(x * s, y * s, z * s); }
    Vector3 operator / (float s) const { return Vector3(x / s, y / s, z / s); }

    friend Vector3 operator + (float s, const Vector3 &vec) { return Vector3(s + vec.x, s + vec.y, s + vec.z); }
    friend Vector3 operator - (float s, const Vector3 &vec) { return Vector3(s - vec.x, s - vec.y, s - vec.z); }
    friend Vector3 operator * (float s, const Vector3 &vec) { return Vector3(s * vec.x, s * vec.y, s * vec.z); }
    friend Vector3 operator / (float s, const Vector3 &vec) { return Vector3(s / vec.x, s / vec.y, s / vec.z); }

    Vector3 &operator += (const Vector3 &vec) { return *this = *this + vec; }
    Vector3 &operator -= (const Vector3 &vec) { return *this = *this - vec; }
    Vector3 &operator *= (const Vector3 &vec) { return *this = *this * vec; }
    Vector3 &operator /= (const Vector3 &vec) { return *this = *this / vec; }
    Vector3 &operator += (float s) { return *this = *this + s; }
    Vector3 &operator -= (float s) { return *this = *this - s; }
    Vector3 &operator *= (float s) { return *this = *this * s; }
    Vector3 &operator /= (float s) { return *this = *this / s; }

    bool operator == (const Vector3 &vec) const { return x == vec.x && y == vec.y && z == vec.z; }
    bool operator != (const Vector3 &vec) const { return x != vec.x || y != vec.y || z != vec.z; }

    float lengthSquared() const { return x * x + y * y + z * z; }
    float length() const { return sqrtf(lengthSquared()); }
    float dot(const Vector3 &vec) const { return x * vec.x + y * vec.y + z * vec.z; }
    Vector3 cross(const Vector3 &vec) const { return Vector3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x); }
    Vector3 unit() const { return *this / length(); }
    void normalize() { *this = unit(); }
    static Vector3 lerp(const Vector3 &a, const Vector3 &b, float percent) { return a + (b - a) * percent; }

    float min() const { return ::min(x, ::min(y, z)); }
    float max() const { return ::max(x, ::max(y, z)); }
    static Vector3 min(const Vector3 &a, const Vector3 &b) { return Vector3(::min(a.x, b.x), ::min(a.y, b.y), ::min(a.z, b.z)); }
    static Vector3 max(const Vector3 &a, const Vector3 &b) { return Vector3(::max(a.x, b.x), ::max(a.y, b.y), ::max(a.z, b.z)); }

    static const Vector3 X;
    static const Vector3 Y;
    static const Vector3 Z;
};

inline std::ostream &operator << (std::ostream &out, const Vector3 &v) { return out << "<" << v.x << ", " << v.y << ", " << v.z << ">"; }

#endif // VECTOR_H
