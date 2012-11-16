/**<!-------------------------------------------------------------------->
   @brief
      Provides basic functionality for a constant-sized Vector.
   <!-------------------------------------------------------------------->**/

#ifndef __CS123_VECTOR_INL__
#define __CS123_VECTOR_INL__

#include <assert.h>
#include <iostream>
using namespace std;


// Extra operators where Vector is on right-hand side
// --------------------------------------------------

//! @returns the N-length vector resulting from multiplying a scalar by an N-length vector
template<typename T>
vec2<T> operator* (const T &scale, const vec2<T> &rhs) { return rhs * scale; }

//! @returns the N-length vector resulting from multiplying a scalar by an N-length vector
template<typename T>
vec3<T> operator* (const T &scale, const vec3<T> &rhs) { return rhs * scale; }

//! @returns the N-length vector resulting from multiplying a scalar by an N-length vector
template<typename T>
vec4<T> operator* (const T &scale, const vec4<T> &rhs) { return rhs * scale; }




//! @returns (-1) * rhs, which is a negated version of the original right-hand side vector
template<typename T>
vec2<T> operator- (const vec2<T> &rhs) { return rhs * (-1); }

//! @returns (-1) * rhs, which is a negated version of the original right-hand side vector
template<typename T>
vec3<T> operator- (const vec3<T> &rhs) { return rhs * (-1); }

//! @returns (-1) * rhs, which is a negated version of the original right-hand side vector
template<typename T>
vec4<T> operator- (const vec4<T> &rhs) { return rhs * (-1); }


// Prints a Vector to an output stream
template <typename T>
inline std::ostream &operator<<(std::ostream &os, const vec2<T> &v) {
   os << "[ " << v.x << " " << v.y << " ]";
   return os;
}
template <typename T>
inline std::ostream &operator<<(std::ostream &os, const vec3<T> &v) {
   os << "[ " << v.x << " " << v.y << " " << v.z << " ]";
   return os;
}
template <typename T>
inline std::ostream &operator<<(std::ostream &os, const vec4<T> &v) {
   os << "[ " << v.x << " " << v.y << " " << v.z << " " << v.w << " ]";
   return os;
}


template<typename T>
vec3<T> vec3<T>::cross(const vec3<T> &rhs) const {
    return vec3<T>(data[1] * rhs.data[2] - data[2] * rhs.data[1],
                   data[2] * rhs.data[0] - data[0] * rhs.data[2],
                   data[0] * rhs.data[1] - data[1] * rhs.data[0]);
};

template<typename T>
vec3<T> vec3<T>::reflectVector(const vec3 &normal) const {

    // @TODO: [RAY] Implement this function for reflection...

    // begin ta code
    return (*this) - (normal * ((*this).dot(normal))) * 2.0;
    // end ta code

}

template<typename T>
vec3<T> vec3<T>::refractVector(const vec3 &normal, T in, T out) const {

    // @TODO: [RAY] You can implement refraction for extra credit.

    // begin ta code
    {

        T n = (sin(out) / sin(in)); //Ratio of indexes of refraction (Snells law)

        /*
            N - normal U - incident vector, n - ratio of refraction indexes
            The full equation would be nI-N(n(N DOT U) + sqrt(1-n^2(1-(N DOT U)^2)))
            However, this can be simplified at the cost of some accuracy to a simpler equation
            U-(1-n)N(N DOT U)
            Taken from Advanced graphics programming using openGL Vol 385.
            Tom McReynolds, David Blythe

            Assuming *this is incident to the surface, and the result is
            pointing away from the surface
        */

        return (*this) - normal*(normal.dot(*this)) * (1 - n);

    }
    // end ta code
};

template<typename T>
inline T vec4<T>::normalize() {
    T m = (T)1.0 / sqrt(x*x + y*y + z*z + w*w);
    #pragma vector align
    for (unsigned i = 0; i < 4; ++i)
        data[i] *= m;
    return 1.0/m;
};


#endif // __CS123_VECTOR_INL__

