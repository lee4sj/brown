#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#define SAFE_DELETE(x) if((x)) { delete (x); (x) = NULL; }
#define MAX(x, y) (x) > (y) ? (x) : (y)
#define MIN(x, y) (x) < (y) ? (x) : (y)
#define PI 3.141592653589f

/**
  * Returns a uniformly distributed random number on the given interval.
  * ex. urand(-1, 1)  would return a random number between -1 and 1.
  */
static inline float urand(float lower = 0.0f, float upper = 1.0f)
{
    return (rand() % 1000) / 1000.0f * (upper - lower) + lower;
}

struct __attribute__ (aligned (16))) float3
{
    union
    {
        struct { float x, y, z; };
        struct { float r, g, b; };
        float data[3];
    };

    float3(float v0 = 0, float v1 = 0, float v2 = 0) : x(v0), y(v1), z(v2) { }
    float3(float *data) { x = data[0]; y = data[1]; z = data[2]; }

    inline void zero() { x = 0; y = 0; z = 0; }
    static inline float3 Zero() { return float3(0,0,0); }

    #define VECOP_PCW(op) { x op rhs.x; y op rhs.y; z op rhs.z; return *this; }
    #define VECOP_SCA(op) { x op rhs;   y op rhs  ; z op rhs  ; return *this; }

    inline float3& operator  = (const float3& rhs) VECOP_PCW( =) /// equality assignment
    inline float3& operator += (const float3& rhs) VECOP_PCW(+=) /// piecewise addition operator
    inline float3& operator -= (const float3& rhs) VECOP_PCW(-=) /// piecewise subtraction operator


    inline float3  operator  + (const float3& rhs) const { return float3(*this) += rhs; } /// piecewise addition
    inline float3  operator  - (const float3& rhs) const { return float3(*this) -= rhs; } /// piecewise subtraction

    inline float3& operator += (const float  rhs)  VECOP_SCA(+=) /// scalar addition operator
    inline float3& operator -= (const float  rhs)  VECOP_SCA(-=) /// scalar subtraction operator
    inline float3& operator *= (const float  rhs)  VECOP_SCA(*=) /// scalar multiplication operator
    inline float3& operator /= (const float  rhs)  VECOP_SCA(/=) /// scalar division operator

    inline float3  operator  + (const float  rhs) const { return float3(*this) += rhs; } /// piecewise addition
    inline float3  operator  - (const float  rhs) const { return float3(*this) -= rhs; } /// piecewise subtraction
    inline float3  operator  * (const float  rhs) const { return float3(*this) *= rhs; } /// piecewise multiplication
    inline float3  operator  / (const float  rhs) const { return float3(*this) /= rhs; } /// piecewise multiplication

    #undef VECOP_PCW
    #undef VECOP_SCA

    inline float dot(const float3 &rhs) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    inline float normalize()
    {
        float scale = 1.0 / getMagnitude();
        x *= scale;
        y *= scale;
        z *= scale;

        return scale;
    }

    inline float3 getNormalized()
    {
        float scale = 1.0 / getMagnitude();
        return float3(x * scale, y * scale, z * scale);
    }

    inline float getMagnitude() const
    {
        return sqrt(getMagnitude2());
    }

    inline float getMagnitude2() const
    {
        return x * x + y * y + z * z;
    }

    inline float getDistance(const float3 &rhs) const
    {
        return sqrt(getDistance2(rhs));
    }

    inline float getDistance2(const float3 &rhs) const
    {
        return (rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y) + (rhs.z - z) * (rhs.z - z);
    }

    inline bool operator==(const float3 &rhs)
    {
        return (x == rhs.x && y == rhs.y && z == rhs.z);
    }

    inline bool operator!=(const float3 &rhs)
    {
        return (x != rhs.x || y != rhs.y || z != rhs.z);
    }
};

inline float3 operator*(const float scale, const float3 &rhs)
{
    return float3(rhs.x * scale, rhs.y * scale, rhs.z * scale);
}

inline float3 operator-(const float3 &rhs)
{
    return float3(-rhs.x, -rhs.y, -rhs.z);
}

inline std::ostream& operator<<(std::ostream& os, const float3& f)
{
        os <<"[";
        for (unsigned i = 0; i < 3; ++i)
            os << f.data[i] << ",";
        os << "]";
        return os;
}

struct float2
{
    union
    {
        struct { float x, y; };
        float data[2];
    };
};

struct Camera
{
    float3 eye, center, up;
    float angle, near, far;
};

#endif // COMMON_H
