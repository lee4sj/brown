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
    m_skybox = ResourceLoader::loadSkybox();
    cout << "Loaded skybox..." << endl;

    loadCubeMap();
    cout << "Loaded cube map..." << endl;

    const QGLContext *ctx = context();
    m_shaderPrograms["bump"] = ResourceLoader::newShaderProgram(ctx, "../projects/final/bump.vert",
                                                                "../projects/final/bump.frag");
    m_shaderPrograms["reflect"] = ResourceLoader::newShaderProgram(ctx, "../final/shaders/reflect.vert",
                                                                   "../final/shaders/reflect.frag");
    m_shaderPrograms["refract"] = ResourceLoader::newShaderProgram(ctx, "../final/shaders/refract.vert",
                                                                   "../final/shaders/refract.frag");




    //prepare textures
    QImage bump_base, bump_temp;
    GLuint textureId;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

////    bump_base.load("textures/ship_texture.png");
//    bump_base.load("textures/NormalMap.jpg");
//    bump_base = bump_base.mirrored(false,true);
//    bump_temp = QGLWidget::convertToGLFormat(bump_base);
////    bump_temp = bump_temp.scaledToWidth(1024,Qt::SmoothTransformation);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bump_temp.width(), bump_temp.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bump_temp.bits());
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    m_textures["bump"] = textureId;


    // Make sure the image file exists
//    QFile file("../projects/textures/NormalMap.jpg");
//    if (!file.exists()) {
//        cout << "was not able to open texture file" << endl;
//        exit(-1);
//    }

//    // Load the file into memory
//    QImage image;
//    image.load(file.fileName());
//    image = image.mirrored(false, true);
//    QImage texture = QGLWidget::convertToGLFormat(image);

//    // Generate a new OpenGL texture ID to put our image into
//    GLuint id = 0;
//    glGenTextures(1, &id);

//    // Make the texture we just created the new active texture
//    glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);

//    // Copy the image data into the OpenGL texture
//    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

//    // Set filtering options
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    // Set coordinate wrapping options
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//    glBindTexture(GL_TEXTURE_2D, 0);

    m_textures["bump"] = loadTexture(QString("../projects/textures/NormalMap.jpg"));

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
    glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);

    // Copy the image data into the OpenGL texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    // Set filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set coordinate wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

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
//        glCallList(m_skybox);

        glEnable(GL_CULL_FACE);

//        m_shaderPrograms["refract"]->bind();
//        m_shaderPrograms["refract"]->setUniformValue("CubeMap", GL_TEXTURE0);

        m_shaderPrograms["bump"]->bind();
        m_shaderPrograms["bump"]->setUniformValue("diffuseTexture", m_textures["bump"]);
        m_shaderPrograms["bump"]->setUniformValue("normalTexture", m_textures["bump"]);

        glPushMatrix();

        glScene->render(this);

        glPopMatrix();
//        m_shaderPrograms["refract"]->release();
        m_shaderPrograms["bump"]->release();

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
