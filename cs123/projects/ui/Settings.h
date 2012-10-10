/**
 @file    Settings.h

 @author  Evan Wallace (edwallac@cs.brown.edu)
 @author  Ben Herila (ben@herila.net)
 @date    9/1/2010

 @brief
    This file contains various settings and enumerations that you will need to
 use in the various assignments. The settings are bound to the GUI via static
 data bindings.

**/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <CS123Algebra.h>

//! Enumeration values for the Brush types from which the user can choose in the GUI.
enum BrushType
{
    BRUSH_SOLID,
    BRUSH_LINEAR,
    BRUSH_QUADRATIC,
    BRUSH_SMUDGE,
    BRUSH_SPECIAL_1,
    BRUSH_SPECIAL_2,
    NUM_BRUSH_TYPES
};

//! Enumeration values for the Filters that the user can select in the GUI.
enum FilterType
{
    FILTER_INVERT,
    FILTER_GREYSCALE,
    FILTER_EDGE_DETECT,
    FILTER_BLUR,
    FILTER_SCALE,
    FILTER_ROTATE,
    FILTER_SPECIAL_1,
    FILTER_SPECIAL_2,
    FILTER_SPECIAL_3,
    NUM_FILTER_TYPES
};

//! Enumeration values for the Shapes that the user can select in the GUI.
enum ShapeType
{
    SHAPE_CUBE,
    SHAPE_CONE,
    SHAPE_SPHERE,
    SHAPE_CYLINDER,
    SHAPE_SPECIAL_1,
    SHAPE_SPECIAL_2,
    SHAPE_SPECIAL_3,
    NUM_SHAPE_TYPES
};

/*! Enumeration values for the different interaction modes that the user
  can use. You'll implement these in the Modelling lab. */
enum InteractionMode
{
    OBJ_FILM_PLANE_TRANSLATE,
    OBJ_LOOK_VECTOR_TRANSLATE,
    OBJ_TRACKBALL_ROTATE,
    OBJ_UNIFORM_SCALE,
    NUM_OBJ_TOOLS
};

//! Enumeration values for the two tabs (2D, 3D) at the bottom of the Window.
enum UITab
{
    TAB_2D,
    TAB_3D,
    NUM_TABS
};

//! Enumeration values for the currently selected scene type
enum SceneMode
{
    SCENEMODE_SHAPES,
    SCENEMODE_SCENEVIEW
};

//! Enumeration values for the currently selected camera type
enum CameraMode
{
    CAMERAMODE_ORBIT,
    CAMERAMODE_CAMTRANS
};

/*!

    @struct Settings
    @brief  Stores application settings for the CS123 GUI.

        You can access all app settings through the "settings" global variable.
    The settings will be automatically updated when things are changed in the
    GUI (the reverse is not true however: changing the value of a setting does
    not update the GUI).

*/
struct Settings
{
    /*! Loads settings from disk, or fills in default values if no saved
      settings exist.  */
    void loadSettingsOrDefaults();

    //! Saves the current settings to disk.
    void saveSettings();

    // Brush
    int brushType;      /// The user's selected brush @see BrushType
    int brushRadius;    /// The brush radius
    int brushRed;       /// The current brush's red channel intensity
    int brushGreen;     /// The current brush's green channel intensity
    int brushBlue;      /// The current brush's blue channel intensity
    int brushAlpha;     /// The current brush's alpha channel (or "flow rate")

    // Filter
    int filterType;             /// The selected filter @see FilterType
    REAL edgeDetectThreshold;   /// Edge detection threshold, from 0 to 1.
    int blurRadius;             /// Selected blur radius
    REAL scaleX;                /// Horizontal scale factor
    REAL scaleY;                /// Vertical scale factor
    int rotateAngle;            /// Rotation angle in degrees

    // Shapes
    bool useSceneviewScene;
    int shapeType;              /// Selected shape type @see ShapeType
    int shapeParameter1;
    int shapeParameter2;
    REAL shapeParameter3;
    bool useLighting;           /// Enable default lighting
    bool drawWireframe;         /// Draw wireframe only
    bool drawNormals;           /// Turn normals on and off

    // Camtrans
    bool useOrbitCamera;        /// Use the built-in orbiting camera instead of the Camtrans perspective camera
    REAL cameraPosX;
    REAL cameraPosY;
    REAL cameraPosZ;
    REAL cameraRotU;
    REAL cameraRotV;
    REAL cameraRotN;
    REAL cameraFov;             /// The camera's field of view. The field of view is twice the height angle.
    REAL cameraNear;            /// The distance from the camera's position to the near clipping plane.
    REAL cameraFar;             /// The distance from the camera's position to the far clipping plane.

    // Modeler
    int objTool;                /// The currently selected modeler tool. @see InteractionMode

    // Ray
    bool useSuperSampling;      /// Enable or disable super-sampling.
    int numSuperSamples;        /// Controls the number of samples per pixel.
    bool useAntiAliasing;       /// Enable or disable anti-aliasing.
    bool useShadows;            /// Enable or disable shadows.
    bool useTextureMapping;     /// Enable or disable texture mapping.
    bool useReflection;         /// Enable or disable reflection.
    bool useRefraction;         /// Enable or disable refraction (this is extra credit).
    bool useMultiThreading;     /// Enable or disable multi-threading (extra credit).
    bool usePointLights;        /// Enable or disable point lighting.
    bool useDirectionalLights;  /// Enable or disable directional lighting (extra credit).
    bool useSpotLights;         /// Enable or disable spot lights (extra credit).

    int getSceneMode();
    int getCameraMode();

    int currentTab;

};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
