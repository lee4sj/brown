/*!
   @file   CS123Matrix.cpp
   @author Travis Fischer (fisch0920@gmail.com)
   @date   Fall 2008
   
   @brief
      Provides basic functionality for a templated, arbitrarily-sized matrix.
      You will need to fill this file in for the Camtrans assignment.

**/

#include "CS123Algebra.h"
#include <iostream>

//@name Routines which generate specific-purpose transformation matrices
//@{---------------------------------------------------------------------
// @returns the scale matrix described by the vector
Matrix4x4 getScaleMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}

// @returns the translation matrix described by the vector
Matrix4x4 getTransMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}

// @returns the rotation matrix about the x axis by the specified angle
Matrix4x4 getRotXMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}

// @returns the rotation matrix about the y axis by the specified angle
Matrix4x4 getRotYMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}

// @returns the rotation matrix about the z axis by the specified angle
Matrix4x4 getRotZMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}

// @returns the rotation matrix around the vector and point by the specified angle
Matrix4x4 getRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}


// @returns the inverse scale matrix described by the vector
Matrix4x4 getInvScaleMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}

// @returns the inverse translation matrix described by the vector
Matrix4x4 getInvTransMat(const Vector4 &v) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}

// @returns the inverse rotation matrix about the x axis by the specified angle
Matrix4x4 getInvRotXMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}

// @returns the inverse rotation matrix about the y axis by the specified angle
Matrix4x4 getInvRotYMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}

// @returns the inverse rotation matrix about the z axis by the specified angle
Matrix4x4 getInvRotZMat (const REAL radians) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}

// @returns the inverse rotation matrix around the vector and point by the specified angle
Matrix4x4 getInvRotMat  (const Vector4 &p, const Vector4 &v, const REAL a) {

    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();

}


//@}---------------------------------------------------------------------

