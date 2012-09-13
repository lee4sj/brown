#include "OpenGLScene.h"
#include "Settings.h"
#include "Camera.h"

#include <SupportCanvas3D.h>
#include <qgl.h>

OpenGLScene::OpenGLScene()
{
}

OpenGLScene::~OpenGLScene()
{
}

void OpenGLScene::render(SupportCanvas3D *context)
{
    // Get the active camera
    Camera *camera = context->getCamera();
    assert(camera);

    // Apply the camera settings
    double matrix[16];
    camera->getProjectionMatrix().getTranspose().fillArray(matrix);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
    camera->getModelviewMatrix().getTranspose().fillArray(matrix);
    glLoadMatrixd(matrix);

    setLights(context->getCamera());

    // Clear the screen in preparation for the next frame (use a gray background instead of a
    // black one for drawing wireframe or normals so they will show up against the background)
    if (settings.drawWireframe || settings.drawNormals) glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    else glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Fill the geometry with white
    glColor3f(1, 1, 1);
    if (settings.useLighting)
    {
        glDisable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
    }
    renderGeometry(true);
    if (settings.useLighting)
    {
        glDisable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
    }

    // Outline the geometry with black
    if (settings.drawWireframe)
    {
        glColor3f(0, 0, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        renderGeometry(false);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (settings.drawNormals)
    {
        // Calculate an eye position so we can do billboarded normals
        double modelview[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        m_cameraEye = Vector4(0, 0, 0, 1) * Matrix4x4(modelview).getInverse();

        // Render normals in black
        glColor3f(0, 0, 0);
        renderNormals();
    }
}

void OpenGLScene::renderNormal(const Vector4 &vertex, const Vector4 &direction)
{
    Vector4 normalDirection = direction.getNormalized();

    // Draw a normal with a fixed length of 0.15
    glBegin(GL_LINES);
    glVertex3dv(vertex.data);
    glVertex3dv((vertex + normalDirection * 0.1).data);
    glEnd();

    // End the normal with an axis-aligned billboarded triangle (billboarding means always rotating
    // to face the camera, and axis-aligned means it can only rotate around the axis of the normal)
    Vector4 triangleVector = direction.cross(m_cameraEye - vertex);
    if (triangleVector.getMagnitude2() > 1.0e-6f)
    {
        triangleVector = triangleVector.getNormalized() * 0.01;
        glBegin(GL_TRIANGLES);
        glVertex3dv((vertex + normalDirection * 0.15).data);
        glVertex3dv((vertex + normalDirection * 0.1 - triangleVector).data);
        glVertex3dv((vertex + normalDirection * 0.1 + triangleVector).data);
        glEnd();
    }
}

void OpenGLScene::applyMaterial(const CS123SceneMaterial &material)
{
    // Make sure the members of CS123SceneColor are packed tightly
    COMPILE_TIME_ASSERT(sizeof(CS123SceneColor) == sizeof(float) * 4);

    if (settings.useLighting)
    {
        // Use materials when lighting is enabled
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &material.cAmbient.r);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &material.cDiffuse.r);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &material.cSpecular.r);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &material.cEmissive.r);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
    }
    else
    {
        // Materials don't work when lighting is disabled, use colors instead
        glColor3fv(&material.cDiffuse.r);
    }
}

void OpenGLScene::setLight(const CS123SceneLightData &light)
{
    // Zero ambient light because the ambient light for the scene is
    // set with GL_LIGHT_MODEL_AMBIENT
    float zero[] = { 0, 0, 0, 0 };

    // There are 8 lights in OpenGL, GL_LIGHT0 to GL_LIGHT7, and
    // each GL_LIGHT* is one greater than the previous one.  This
    // means the nth light is GL_LIGHT0 + n.
    int id = GL_LIGHT0 + light.id;

    glLightfv(id, GL_AMBIENT, zero);
    glLightfv(id, GL_DIFFUSE, &light.color.r);
    glLightfv(id, GL_SPECULAR, &light.color.r);
    glLightf(id, GL_CONSTANT_ATTENUATION, light.function.data[0]);
    glLightf(id, GL_LINEAR_ATTENUATION, light.function.data[1]);
    glLightf(id, GL_QUADRATIC_ATTENUATION, light.function.data[2]);

    if (glIsEnabled(id))
        cout << "warning: GL_LIGHT" << light.id << " enabled more than once, not supposed to happen" << endl;

    switch (light.type)
    {
    case LIGHT_POINT:
        {
            // Convert from double[] to float[] and make sure the w coordinate is
            // correct (the CS123 scene loader gets it wrong for point lights)
            float position[] = { light.pos.data[0], light.pos.data[1], light.pos.data[2], 1 };
            glLightfv(id, GL_POSITION, position);
            if (settings.usePointLights) glEnable(id);
            break;
        }

    case LIGHT_DIRECTIONAL:
        {
            // Convert from double[] to float[] and make sure the direction vector
            // is normalized (it isn't for a lot of scene files)
            Vector4 direction = -light.dir.getNormalized();
            float position[] = { direction.data[0], direction.data[1], direction.data[2], direction.data[3] };
            glLightfv(id, GL_POSITION, position);
            if (settings.useDirectionalLights) glEnable(id);
            break;
        }

    default:
        break;
    }
}

