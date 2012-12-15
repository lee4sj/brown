#include "Canvas3D.h"
#include <Settings.h>
#include <typeinfo>
#include <Camera.h>
#include <OrbitingCamera.h>
#include <OpenGLScene.h>

#include <QDir>

#include "final/resourceloader.h"

Canvas3D::Canvas3D(QWidget *parent) : SupportCanvas3D(parent)
{


}

Canvas3D::~Canvas3D()
{


}


void Canvas3D::initializeGL()
{
    //final
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glDisable(GL_DITHER);

    glDisable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);


    // Track the camera settings so we can generate deltas
    m_oldPosX = settings.cameraPosX;
    m_oldPosY = settings.cameraPosY;
    m_oldPosZ = settings.cameraPosZ;
    m_oldRotU = settings.cameraRotU;
    m_oldRotV = settings.cameraRotV;
    m_oldRotN = settings.cameraRotN;

    // Use a depth buffer and only draw when objects have a depth less than or
    // equal to the depth present in the buffer.  This will make sure the final
    // buffer will only contain the closest object at each pixel (the depth
    // buffer starts off filled with the furthest possible distance).
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Move the polygons back a bit so lines are still drawn even though they are
    // coplanar with the polygons they came from, which will be drawn before them
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1, -1);

    // Specular reflections will be off without this, since OpenGL calculates
    // specular highlights using an infinitely far away camera by default, not
    // the actual location of the camera
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

    // TODO: might want to do this for textured scenes
    //glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    // Hide backfaces so you can tell when triangles are backwards (counter-clockwise
    // triangles will be visible)
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Show all ambient light for the entire scene (not one by default)
    static float one[] = { 1, 1, 1, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);

    initializeResources();

    // Calculate the orbiting camera matrices now that the OpenGL context is created.
    getOrbitingCamera()->updateMatrices();
}

void Canvas3D::initializeResources()
{
//    m_skybox = ResourceLoader::loadSkybox();
//    cout << "Loaded skybox..." << endl;

//    loadCubeMap();
//    cout << "Loaded cube map..." << endl;

    const QGLContext *ctx = context();
    m_shaderPrograms["bump"] = ResourceLoader::newShaderProgram(ctx, "../projects/final/bump.vert",
                                                                "../projects/final/bump.frag");
    m_shaderPrograms["leaf"] = ResourceLoader::newShaderProgram(ctx, "../projects/final/leaf.vert",
                                                                "../projects/final/leaf.frag");

//    m_shaderPrograms["reflect"] = ResourceLoader::newShaderProgram(ctx, "../final/shaders/reflect.vert",
//                                                                   "../final/shaders/reflect.frag");
//    m_shaderPrograms["refract"] = ResourceLoader::newShaderProgram(ctx, "../final/shaders/refract.vert",
//                                                                   "../final/shaders/refract.frag");




    //prepare textures
    m_textures["normalMap"] = loadTexture(QString("../projects/textures/NormalMap.jpg"));
    m_textures["treeTexture"] = loadTexture(QString("../projects/textures/normal1.jpg"));
    m_textures["leafTexture"] = loadTexture(QString("../projects/textures/leaf3.jpg"));
}

GLuint Canvas3D::loadTexture(const QString &filename)
{
    // Make sure the image file exists
    QFile file(filename);
    if (!file.exists()) {
        cout << "load failed" << endl;
        return -1;
    }

    // Load the file into memory
    QImage image;
    image.load(file.fileName());
    image = image.mirrored(false, true);
    QImage texture = QGLWidget::convertToGLFormat(image);

    // Generate a new OpenGL texture ID to put our image into
    GLuint id = 0;
    glGenTextures(1, &id);

    // Make the texture we just created the new active texture
    glBindTexture(GL_TEXTURE_2D, id);

    // Copy the image data into the OpenGL texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    // Set filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set coordinate wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glBindTexture(GL_TEXTURE_2D, 0);

    return id;
}


/**
  Load a cube map for the skybox
 **/
void Canvas3D::loadCubeMap()
{
    QList<QFile *> fileList;
    fileList.append(new QFile("../projects/textures/astra/posx.jpg"));
    fileList.append(new QFile("../projects/textures/astra/negx.jpg"));
    fileList.append(new QFile("../projects/textures/astra/posy.jpg"));
    fileList.append(new QFile("../projects/textures/astra/negy.jpg"));
    fileList.append(new QFile("../projects/textures/astra/posz.jpg"));
    fileList.append(new QFile("../projects/textures/astra/negz.jpg"));
    m_cubeMap = ResourceLoader::loadCubeMap(fileList);
}


void Canvas3D::paintGL()
{
    OpenGLScene *glScene = this->getScene();

    if (glScene != NULL)
    {

        // Check for errors from the last frame
        int err;
        if ((err = glGetError()) != GL_NO_ERROR) {
            fprintf(stderr, "GL is in an error state before painting.\n");
            fprintf(stderr, "(GL error code %d)\n", err);
            //assert(0);
        }

        // Update the scene camera
        glViewport(0, 0, width(), height());
        getCamera()->setAspectRatio((float)width() / (float)height());

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Enable cube maps and draw the skybox
        glEnable(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
        glDisable(GL_LIGHTING);
//        glPushMatrix();
//        glLoadIdentity();
//        glCallList(m_skybox);
//        glPopMatrix();
        glEnable(GL_LIGHTING);
        glEnable(GL_CULL_FACE);

        /*------------------------------*/
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, m_textures["normalMap"]);

//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_2D, m_textures["treeTexture"]);

//        m_shaderPrograms["bump"]->bind();
//        m_shaderPrograms["bump"]->setUniformValue("normalMap", 0);
//        m_shaderPrograms["bump"]->setUniformValue("treeTexture", 1);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


        glPushMatrix();
        glScene->render(this);
        glPopMatrix();
//        m_shaderPrograms["bump"]->release();

        glDisable(GL_BLEND);
        /*-------------------------------*/

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_CUBE_MAP,0);
        glDisable(GL_TEXTURE_CUBE_MAP);

    }
    else
    {

        // Display a placeholder message explaining that no OpenGLScene is
        // currently loaded.

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1, 1, 1);
        renderText(width() / 2 - 80, height() / 2 + 5, "No OpenGLScene loaded");

    }
}


void Canvas3D::settingsChanged() {

    // TODO: Process changes to the application settings.


    // Call superclass (this repaints the scene for you)
    SupportCanvas3D::settingsChanged();
}
