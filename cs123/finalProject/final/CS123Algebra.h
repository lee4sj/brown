/*!
   @file   CS123Algebra.h
   @author Travis Fischer (fisch0920@gmail.com)
   @date   Fall 2008

   @brief
      Convenience header which defines all types in the CS123 linear algebra package
**/

#ifndef __CS123_ALGEBRA_H__
#define __CS123_ALGEBRA_H__

#include <math.h>

typedef double REAL;

#define CS123_VECTOR_NO_ELEMENTS    (N)
#define CS123_VECTOR_SIZE           (sizeof(T) * CS123_VECTOR_NO_ELEMENTS)
#define CS123_MATRIX_NO_ELEMENTS    (M * N)
#define CS123_MATRIX_SIZE           (sizeof(T) * CS123_MATRIX_NO_ELEMENTS)

#define SQ(x) (x)*f(x)

/* These vector operations are used in all other vector classes, so we use the compiler
to make all the vector operations generic enough that they can be used with specific
classes. We could use template classes, but that would not allow us to access vectors
by vector.x, vector.y, vector.z, etc. Thus, we are forced to declare the structures
separately. The operations are generated based on the preprocessor values of
VECOP_PCW, VECOP_SCA, and X. */

#define VECTOR_OPS  \
    inline X& operator  = (const X& rhs) VECOP_PCW( =) /*<<< equality assignment */ \
    inline X& operator += (const X& rhs) VECOP_PCW(+=) /*<<< piecewise addition operator */ \
    inline X& operator -= (const X& rhs) VECOP_PCW(-=) /*<<< piecewise subtraction operator */ \
    inline X& operator *= (const X& rhs) VECOP_PCW(*=) /*<<< piecewise multiplication operator */ \
    inline X& operator /= (const X& rhs) VECOP_PCW(/=) /*<<< piecewise division operator */ \
     \
    inline X  operator  + (const X& rhs) const { return X(*this) += rhs; } /*<<< piecewise addition */ \
    inline X  operator  - (const X& rhs) const { return X(*this) -= rhs; } /*<<< piecewise subtraction */ \
    inline X  operator  * (const X& rhs) const { return X(*this) *= rhs; } /*<<< piecewise multiplication */ \
    inline X  operator  / (const X& rhs) const { return X(*this) /= rhs; } /*<<< piecewise division */ \
     \
    inline X& operator += (const T  rhs)  VECOP_SCA(+=) /*<<< scalar addition operator */ \
    inline X& operator -= (const T  rhs)  VECOP_SCA(-=) /*<<< scalar subtraction operator */ \
    inline X& operator *= (const T  rhs)  VECOP_SCA(*=) /*<<< scalar multiplication operator */ \
    inline X& operator /= (const T  rhs)  VECOP_SCA(/=) /*<<< scalar division operator */ \
     \
    inline X  operator  + (const T  rhs) const { return X(*this) += rhs; } /*<<< piecewise addition */ \
    inline X  operator  - (const T  rhs) const { return X(*this) -= rhs; } /*<<< piecewise subtraction */ \
    inline X  operator  * (const T  rhs) const { return X(*this) *= rhs; } /*<<< piecewise multiplication */ \
    inline X  operator  / (const T  rhs) const { return X(*this) /= rhs; } /*<<< piecewise multiplication */ \

template<typename T = REAL>
struct vec2 {

///@name Constructors
//@{-----------------------------------------------------------------

    vec2<T>() {}
    vec2<T>(T v0, T v1) : x(v0), y(v1) {}
    vec2<T>(const vec2<T> &copy) : x(copy.x), y(copy.y) {}

    inline void init(T X, T Y) { x=X, y=Y;}
    static vec2<T> zero() { return vec2<T>(0, 0); }

//@}-----------------------------------------------------------------
///@name Operators
//@{-----------------------------------------------------------------

#define VECOP_PCW(op) { x op rhs.x; y op rhs.y; return *this; }
#define VECOP_SCA(op) { x op rhs;   y op rhs  ; return *this; }
#define X vec2

    VECTOR_OPS

    //! Equality operator
    inline bool operator==(const X &rhs) { return (x == rhs.x && y == rhs.y); }

    //! Inequality operator
    inline bool operator!=(const X &rhs) { return (x != rhs.x || y != rhs.y); }


#undef X
#undef VECOP_PCW
#undef VECOP_SCA

//@}-----------------------------------------------------------------
///@name Additional Operations
//@{-----------------------------------------------------------------

//@}-----------------------------------------------------------------

    union {
        T data[2];
        struct { T x, y; };
    };

};

template<typename T = REAL>
struct vec3  {

///@name Constructors
//@{-----------------------------------------------------------------

    vec3(T v0 = 0, T v1 = 0, T v2 = 0) : x(v0), y(v1), z(v2) { }
    vec3(T *data) { x = data[0]; y = data[1]; z = data[2]; }

    inline void init(T X, T Y, T Z) { x=X, y=Y, z=Z; }
    static inline vec3 zero() { return vec3(0,0,0); }

//@}-----------------------------------------------------------------

///@name Operators
//@{-----------------------------------------------------------------

#define VECOP_PCW(op) { x op rhs.x; y op rhs.y; z op rhs.z; return *this; }
#define VECOP_SCA(op) { x op rhs;   y op rhs  ; z op rhs  ; return *this; }
#define X vec3

    VECTOR_OPS /* include the standard vector operations */

    //! Equality operator
    inline bool operator==(const X &rhs) { return (x == rhs.x && y == rhs.y && z == rhs.z); }

    //! Inequality operator
    inline bool operator!=(const X &rhs) { return (x != rhs.x || y != rhs.y || z != rhs.z); }


#undef X
#undef VECOP_PCW
#undef VECOP_SCA

//@}-----------------------------------------------------------------

///@name Additional Operations
//@{-----------------------------------------------------------------

    inline T normalize()                          { T m = getMagnitude(); x /= m, y /= m, z /= m; return m; }

    inline bool isZero()                    const { return EQ(x,0) && EQ(y,0) && EQ(z,0); }
    inline vec3 getNormalized()             const { T m = getMagnitude(); return vec3(x/m, y/m, z/m); }
    inline T getMagnitude()                 const { return sqrt(getMagnitude2()); }
    inline T getMagnitude2()                const { return x*x + y*y + z*z; }

    inline T getDistance(const vec3 &rhs)   const { return sqrt(getDistance2(rhs)); }
    inline T getDistance2(const vec3 &rhs)  const { return SQ(rhs.x - x) + SQ(rhs.y - y) + SQ(rhs.z - z); }

    inline T dot(const vec3 &rhs)           const { return x*rhs.x + y*rhs.y + z*rhs.z; }
    inline vec3 cross(const vec3& rhs)      const;

    /**
       Assuming *this is incident to the surface and the result is pointing
       away from the surface.
    **/
    inline vec3 reflectVector(const vec3 &normal) const;
    inline vec3 refractVector(const vec3 &normal, T in, T out) const;

//@}-----------------------------------------------------------------

    union {
        T data[3];
        struct {
            T x, y, z;
        };
    };

};


template<typename T>
struct vec4 {
#define vec4_T vec4<T>

///@name Constructors
//@{-----------------------------------------------------------------

    inline vec4_T() : x(0), y(0), z(0), w(0) {}
    inline vec4_T(T v0, T v1, T v2, T v3) : x(v0), y(v1), z(v2), w(v3) {}
    inline vec4_T(const T *data)    { x = data[0]; y = data[1]; z = data[2]; w = data[3]; }

    inline void init(T X, T Y, T Z, T W) { x=X, y=Y, z=Z, w=W; }
    static inline vec4_T zero()     { return vec4_T(0,0,0,0); }

//@}-----------------------------------------------------------------

///@name Operators
//@{-----------------------------------------------------------------

#define VECOP_PCW(op) { x op rhs.x; y op rhs.y; z op rhs.z; w op rhs.w; return *this; }
#define VECOP_SCA(op) { x op rhs  ; y op rhs  ; z op rhs  ; w op rhs  ; return *this; }
#define X vec4_T

    VECTOR_OPS /* include the standard vector operations */

    //! Equality operator
    inline bool operator==(const X &rhs) { return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w); }

    //! Inequality operator
    inline bool operator!=(const X &rhs) { return (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w); }

#undef X
#undef VECOP_PCW
#undef VECOP_SCA

//@}-----------------------------------------------------------------

///@name Additional Operations
//@{-----------------------------------------------------------------

    inline vec4_T& homogenize()     { w = 1; return *this; }
    inline vec4_T& unhomgenize()    { w = 0; return *this; }
    //inline T max()                  const { return qMax(qMax(qMax(x, y), z), w); }
    inline T dot(const vec4_T &rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }

    inline T normalize();

    inline vec4_T getNormalized() const {
        T m = 1.0 / sqrt(x*x + y*y + z*z + w*w);
        return vec4_T(x*m, y*m, z*m, w*m);
    }

    inline T getMagnitude() const {
        return sqrt(getMagnitude2());
    }

    inline T getMagnitude2() const {
        return x * x + y * y + z * z + w * w;
    }

    inline T getDistance(const vec4_T &rhs) const {
        return sqrt(getDistance2(rhs));
    }

    inline T getDistance2(const vec4_T &rhs) const {
        return (rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y) +
                (rhs.z - z) * (rhs.z - z) + (rhs.w - w) * (rhs.w - w);
    }

    inline vec4_T getHomogenized() {
        return vec4_T(*this).homogenize();
    }

    inline vec4_T cross(const vec4_T& rhs) const {
        return vec4_T( data[1] * rhs.data[2] - data[2] * rhs.data[1],
                       data[2] * rhs.data[0] - data[0] * rhs.data[2],
                       data[0] * rhs.data[1] - data[1] * rhs.data[0], 0);
    }

//@}-----------------------------------------------------------------

    union {
        T data[4];
        struct {
            T x, y, z, w;
        };
    };
};

//#define Vector2 vec2<REAL>
//#define Vector3 vec3<REAL>
#define Vector4 vec4<REAL>

/* Include inline implementations */
#include "CS123Vector.inl"

template<typename T = REAL>
struct mat4 {
    mat4<T>( T _00 = 0, T _01 = 0, T _02 = 0, T _03 = 0,
             T _10 = 0, T _11 = 0, T _12 = 0, T _13 = 0,
             T _20 = 0, T _21 = 0, T _22 = 0, T _23 = 0,
             T _30 = 0, T _31 = 0, T _32 = 0, T _33 = 0 ){
        data[0] = _00;        data[1] = _01;        data[2] = _02;        data[3] = _03;
        data[4] = _10;        data[5] = _11;        data[6] = _12;        data[7] = _13;
        data[8] = _20;        data[9] = _21;        data[10] = _22;       data[11] = _23;
        data[12] = _30;       data[13] = _31;       data[14] = _32;       data[15] = _33;
    }

    mat4<T>(const T *copy)                 { memcpy(data, copy,      sizeof(T) * 16); }
    mat4<T>(const mat4<T> &copy)           { memcpy(data, copy.data, sizeof(T) * 16); }
    mat4<T>& operator=(const mat4<T> &rhs) { memcpy(data, rhs.data,  sizeof(T) * 16); return *this; }

    inline void fillArray(T* dest) const   { memcpy(dest, data,      sizeof(T) * 16); }

    inline       T& get(int row, int col)       { return data[row * 4 + col]; }
    inline const T& get(int row, int col) const { return data[row * 4 + col]; }

    static mat4<T> identity();
    static mat4<T> transpose(const mat4<T> &m);

    inline mat4<T> getTranspose() const { return mat4<T>::transpose(*this); }

    inline mat4& operator+=(const mat4& rhs) {
#pragma vector align
        for (unsigned i = 0; i < 16; ++i)
            data[i] += rhs.data[i];
        return *this;
    }

    inline mat4& operator-=(const mat4& rhs) {
#pragma vector align
        for (unsigned i = 0; i < 16; ++i)
            data[i] -= rhs.data[i];
        return *this;
    }

    inline mat4 operator-(const mat4 &rhs) {
        return mat4(*this) -= rhs;
    }

    inline mat4 operator+(const mat4 &rhs) {
        return mat4(*this) += rhs;
    }

    inline mat4& operator*=(const T rhs) {
        data[0] *= rhs;
        data[1] *= rhs;
        data[2] *= rhs;
        data[3] *= rhs;
        data[4] *= rhs;
        data[5] *= rhs;
        data[6] *= rhs;
        data[7] *= rhs;
        data[8] *= rhs;
        data[9] *= rhs;
        data[10] *= rhs;
        data[11] *= rhs;
        data[12] *= rhs;
        data[13] *= rhs;
        data[14] *= rhs;
        data[15] *= rhs;
        return *this;
    }

    inline Vector4 operator*(const Vector4 &rhs) const {
        return Vector4( a*rhs.x + b*rhs.y + c*rhs.z + d*rhs.w,
                        e*rhs.x + f*rhs.y + g*rhs.z + h*rhs.w,
                        i*rhs.x + j*rhs.y + k*rhs.z + l*rhs.w,
                        m*rhs.x + n*rhs.y + o*rhs.z + p*rhs.w );
    }

    inline void mulVec4(const Vector4 &rhs, Vector4 &out) {
        out.data[0] = a*rhs.x + b*rhs.y + c*rhs.z + d*rhs.w;
        out.data[1] = e*rhs.x + f*rhs.y + g*rhs.z + h*rhs.w;
        out.data[2] = i*rhs.x + j*rhs.y + k*rhs.z + l*rhs.w;
        out.data[3] = m*rhs.x + n*rhs.y + o*rhs.z + p*rhs.w;
    }

    inline mat4& operator/=(const T rhs) {
        data[0] /= rhs;
        data[1] /= rhs;
        data[2] /= rhs;
        data[3] /= rhs;
        data[4] /= rhs;
        data[5] /= rhs;
        data[6] /= rhs;
        data[7] /= rhs;
        data[8] /= rhs;
        data[9] /= rhs;
        data[10] /= rhs;
        data[11] /= rhs;
        data[12] /= rhs;
        data[13] /= rhs;
        data[14] /= rhs;
        data[15] /= rhs;
        return *this;
    }

    inline mat4 operator/(const T rhs) {
        return mat4(*this) /= rhs;
    }



    inline mat4 getInverse() const {
        return mat4(*this).invert();
    }

    inline T getDeterminant() {
        return  a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o -
                a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m -
                b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p +
                c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n +
                d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m;
    }

    inline mat4& invert() {
        T det = getDeterminant(),
        _00 = (f*k*p+g*l*n+h*j*o-f*l*o-g*j*p-h*k*n)/det,
        _01 = (b*l*o+c*j*p+d*k*n-b*k*p-c*l*n-d*j*o)/det,
        _02 = (b*g*p+c*h*n+d*f*o-b*h*o-c*f*p-d*g*n)/det,
        _03 = (b*h*k+c*f*l+d*g*j-b*g*l-c*h*j-d*f*k)/det,
        _10 = (e*l*o+h*k*m+g*i*p-e*k*p-g*l*m-h*i*o)/det,
        _11 = (a*k*p+c*l*m+d*i*o-a*l*o-c*i*p-d*k*m)/det,
        _12 = (a*h*o+c*e*p+d*g*m-a*g*p-c*h*m-d*e*o)/det,
        _13 = (a*g*l+c*h*i+d*e*k-a*h*k-c*e*l-d*g*i)/det,
        _20 = (e*j*p+f*l*m+h*i*n-e*l*n-f*i*p-h*j*m)/det,
        _21 = (a*l*n+b*i*p+d*j*m-a*j*p-b*l*m-d*i*n)/det,
        _22 = (a*f*p+b*h*m+d*e*n-a*h*n-b*e*p-d*f*m)/det,
        _23 = (a*h*j+b*e*l+d*f*i-a*f*l-b*h*i-d*e*j)/det,
        _30 = (e*k*n+f*i*o+g*j*m-e*j*o-f*k*m-g*i*n)/det,
        _31 = (a*j*o+b*k*m+c*i*n-a*k*n-b*i*o-c*j*m)/det,
        _32 = (a*g*n+b*e*o+c*f*m-a*f*o-b*g*m-c*e*n)/det,
        _33 = (a*f*k+b*g*i+c*e*j-a*g*j-b*e*k-c*f*i)/det;
        a = _00;
        b = _01;
        c = _02;
        d = _03;
        e = _10;
        f = _11;
        g = _12;
        h = _13;
        i = _20;
        j = _21;
        k = _22;
        l = _23;
        m = _30;
        n = _31;
        o = _32;
        p = _33;
        return *this;
    }

    inline mat4& operator*=(const mat4 &rhs) {
        T _00 = data[0]  * rhs.data[0] + data[1]  * rhs.data[4] + data[2]  * rhs.data[8]  + data[3]  * rhs.data[12];
        T _01 = data[0]  * rhs.data[1] + data[1]  * rhs.data[5] + data[2]  * rhs.data[9]  + data[3]  * rhs.data[13];
        T _02 = data[0]  * rhs.data[2] + data[1]  * rhs.data[6] + data[2]  * rhs.data[10] + data[3]  * rhs.data[14];
        T _03 = data[0]  * rhs.data[3] + data[1]  * rhs.data[7] + data[2]  * rhs.data[11] + data[3]  * rhs.data[15];

        T _10 = data[4]  * rhs.data[0] + data[5]  * rhs.data[4] + data[6]  * rhs.data[8]  + data[7]  * rhs.data[12];
        T _11 = data[4]  * rhs.data[1] + data[5]  * rhs.data[5] + data[6]  * rhs.data[9]  + data[7]  * rhs.data[13];
        T _12 = data[4]  * rhs.data[2] + data[5]  * rhs.data[6] + data[6]  * rhs.data[10] + data[7]  * rhs.data[14];
        T _13 = data[4]  * rhs.data[3] + data[5]  * rhs.data[7] + data[6]  * rhs.data[11] + data[7]  * rhs.data[15];

        T _20 = data[8]  * rhs.data[0] + data[9]  * rhs.data[4] + data[10] * rhs.data[8]  + data[11] * rhs.data[12];
        T _21 = data[8]  * rhs.data[1] + data[9]  * rhs.data[5] + data[10] * rhs.data[9]  + data[11] * rhs.data[13];
        T _22 = data[8]  * rhs.data[2] + data[9]  * rhs.data[6] + data[10] * rhs.data[10] + data[11] * rhs.data[14];
        T _23 = data[8]  * rhs.data[3] + data[9]  * rhs.data[7] + data[10] * rhs.data[11] + data[11] * rhs.data[15];

        T _30 = data[12] * rhs.data[0] + data[13] * rhs.data[4] + data[14] * rhs.data[8]  + data[15] * rhs.data[12];
        T _31 = data[12] * rhs.data[1] + data[13] * rhs.data[5] + data[14] * rhs.data[9]  + data[15] * rhs.data[13];
        T _32 = data[12] * rhs.data[2] + data[13] * rhs.data[6] + data[14] * rhs.data[10] + data[15] * rhs.data[14];
        T _33 = data[12] * rhs.data[3] + data[13] * rhs.data[7] + data[14] * rhs.data[11] + data[15] * rhs.data[15];

        data[0] = _00;
        data[1] = _01;
        data[2] = _02;
        data[3] = _03;
        data[4] = _10;
        data[5] = _11;
        data[6] = _12;
        data[7] = _13;
        data[8] = _20;
        data[9] = _21;
        data[10] = _22;
        data[11] = _23;
        data[12] = _30;
        data[13] = _31;
        data[14] = _32;
        data[15] = _33;
        return *this;
    }

    inline mat4 operator*(const mat4& rhs) const {
        return mat4(*this) *= rhs;
    }

    inline mat4 operator*(const T rhs) const {
        return mat4(*this) *= rhs;
    }

    union {
        T data[16];
        struct {
            T a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p;
        };
    };
};

typedef mat4<REAL> Matrix4x4;

/* Include inline implementations */
#include "CS123Matrix.inl"

/* The following global functions are not templated, so they will be found in
 * CS123Matrix.cpp
 */

///@name Routines which generate specific-purpose transformation matrices
//@{-----------------------------------------------------------------------------

/// @returns the scale matrix described by the vector
extern Matrix4x4 getScaleMat(const Vector4 &v);
/// @returns the translation matrix described by the vector
extern Matrix4x4 getTransMat(const Vector4 &v);
/// @returns the rotation matrix about the x axis by the specified angle
extern Matrix4x4 getRotXMat (const REAL radians);
/// @returns the rotation matrix about the y axis by the specified angle
extern Matrix4x4 getRotYMat (const REAL radians);
/// @returns the rotation matrix about the z axis by the specified angle
extern Matrix4x4 getRotZMat (const REAL radians);
/// @returns the rotation matrix around the vector and point by the specified angle
extern Matrix4x4 getRotMat  (const Vector4 &p, const Vector4 &v, const REAL a);

//@}-----------------------------------------------------------------------------

///@name Routines which generate specific-purpose inverse transformation matrices
//@{-----------------------------------------------------------------------------

/// @returns the inverse scale matrix described by the vector
extern Matrix4x4 getInvScaleMat(const Vector4 &v);
/// @returns the inverse translation matrix described by the vector
extern Matrix4x4 getInvTransMat(const Vector4 &v);
/// @returns the inverse rotation matrix about the x axis by the specified angle
extern Matrix4x4 getInvRotXMat (const REAL radians);
/// @returns the inverse rotation matrix about the y axis by the specified angle
extern Matrix4x4 getInvRotYMat (const REAL radians);
/// @returns the inverse rotation matrix about the z axis by the specified angle
extern Matrix4x4 getInvRotZMat (const REAL radians);
/// @returns the inverse rotation matrix around the vector and point by the specified angle
extern Matrix4x4 getInvRotMat  (const Vector4 &p, const Vector4 &v, const REAL a);

//@}-----------------------------------------------------------------------------

#endif // __CS123_ALGEBRA_H__

#include "vector.h"

