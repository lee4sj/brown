/*!
   @file   CS123SceneData.h
   @author Nong Li
   @date   Fall 2008

   @brief  Header file containing scene data structures.
*/

#ifndef __CS123_SCENE_DATA__
#define __CS123_SCENE_DATA__

/* Includes */
#include "math/CS123Algebra.h"
#include <vector>
#include <string>
using namespace std;

//! Enumeration for light types.
enum LightType {
   LIGHT_POINT, LIGHT_DIRECTIONAL, LIGHT_SPOT, LIGHT_AREA
};

//! Struct to store a RGBA color in floats [0,1]
struct CS123SceneColor 
{
    union {
        struct {
           float r;
           float g;
           float b;
           float a;
        };
        float channels[4]; // points to the same four floats above...
    };

   // @TODO: [OPTIONAL] You can implement some operators here for color arithmetic.

};

////! Data for a single light
//struct CS123SceneLightData
//{
//   int id;
//   LightType type;

//   CS123SceneColor color;
//   Vector3 function;    //! Attenuation function

//   Vector4  pos;        //! Not applicable to directional lights
//   Vector4 dir;         //! Not applicable to point lights

//   float radius;        //! Only applicable to spot lights
//   float penumbra;      //! Only applicable to spot lights
//   float angle;         //! Only applicable to spot lights

//   float width, height; //! Only applicable to area lights
//};

//! Data for scene materials
struct CS123SceneMaterial 
{
   /*! @brief This field specifies the diffuse color of the object. 
   This is the color you need to use for the object in sceneview. 
   You can get away with ignoring the other color values until 
   intersect and ray. */ 
   CS123SceneColor cDiffuse;
   
   CS123SceneColor cAmbient;
   CS123SceneColor cReflective;
   CS123SceneColor cSpecular;
   CS123SceneColor cTransparent;
   CS123SceneColor cEmissive;

//   CS123SceneFileMap* textureMap;
   float blend;

//   CS123SceneFileMap* bumpMap;

   float shininess;

   float ior;           //! index of refaction
};

#endif

