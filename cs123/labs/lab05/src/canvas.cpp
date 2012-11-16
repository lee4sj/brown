#include "canvas.h"

#ifdef __APPLE__
#include <OpenGL/glext.h>
#else
#include <GL/glext.h>
#endif

#include <qgl.h>
#include <iostream>

#include <QMouseEvent>
#include <QTimer>
#include <QFile>

#include <cmath>

using namespace std;

// hack to get around windef.h on Windows
#undef near
#undef far

Lab5Canvas::Lab5Canvas(QWidget *parent) : QGLWidget(parent), m_fps(30.0f)
{
    //You may want to add code here
    m_renderNormals = false;
    m_camera = new LabCamera();
    m_camera->eye.x = 0.0f, m_camera->eye.y = 20.f, m_camera->eye.z = 10.0f;
    m_camera->center.x = 0.0f, m_camera->center.y = 0.0f, m_camera->center.z = 0.0f;
    m_camera->up.x = 0.0f, m_camera->up.y = 1.0f, m_camera->up.z = 0.0f;
    m_camera->angle = 45.0f, m_camera->near = 1.0f, m_camera->far = 1000.0f;

    // Determines how much each vertex gets perturbed. The larger the value, the less pertubration will occur per recursive value
    m_decay = 2;

    // The number of levels of recursion your terrain uses (can be considered the level of detail of your terrain)
    m_depth = 2;

    // The roughness of your terrain. Higher roughnesses entail taller mountains and deeper valleys. Lower values entail small hills and shallow valleys
    m_roughness = 6;

    // Seed a random number, your terrain will change based on what number you seed
    srand(0);

    // Do not change below here
    QObject::connect(this, SIGNAL(_mouseDrag(QMouseEvent*)), this, SLOT(mouseDragEvent(QMouseEvent*)));
    setAutoBufferSwap(false);
    setFocusPolicy(Qt::StrongFocus);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(redraw()));
    m_timer->start(1000.0f / m_fps);

    // Imagining your terrain as a square grid of points, gridLength is the number of elements on one side, or the length of a side of the grid
    m_gridLength = (1 << m_depth)+1;

    // Knowing the number of elements on a side gives us the total number of elements in the grid
    int terrain_array_size = m_gridLength * m_gridLength;

    // Represents all of our vertices. Picture this as a grid
    m_terrain = new Vector3[terrain_array_size];

    // Represents all the normals, one per each vertex
    m_normalMap = new Vector3[terrain_array_size];
}

// Destructor
Lab5Canvas::~Lab5Canvas()
{
    if (m_timer)
    {
        delete m_timer;
        m_timer = 0;
    }

    if (m_camera)
    {
        delete m_camera;
        m_camera = 0;
    }

    delete[] m_terrain;
    delete[] m_normalMap;
}

// You will only need to deal with the next 3 (maybe 4) methods

/**
 * You need to fill this in.
 *
 * Subdivides a square by finding the vertices at its corners, the midpoints of each side, and
 * the center (as the algorithm describes). Then recurs on each of the four sub-squares created.
 *
 * @param topLeft The grid coordinate of the top-left corner of the square to subdivide
 * @param bottomRight The grid coordinate of the bottom-right corner of the square to subdivide
 * @param depth The current recursion depth, decreasing as this function recurses deeper. The
 *              function should stop recurring when this value reaches zero.
 */
void Lab5Canvas::subdivideSquare(GridPoint topleft, GridPoint botright, GLint curDepth)
{
    if(curDepth < 0)
    {
        return;
    }

    // TL--TM--TR    +---> x
    // |   |   |     |
    // ML--MM--MR    V
    // |   |   |     y
    // BL--BM--BR

    // corner coordinates (in the grid space [x,y])
    GridPoint TL = GridPoint(topleft.x, topleft.y);
    GridPoint TR = GridPoint(botright.x, topleft.y);
    GridPoint BL = GridPoint(topleft.x, botright.y);
    GridPoint BR = GridPoint(botright.x, botright.y);

    // corner vertices on the terrain (in the grid space [x,y,z])
    Vector3 &vTL = m_terrain[getIndex(TL)];
    Vector3 &vTR = m_terrain[getIndex(TR)];
    Vector3 &vBL = m_terrain[getIndex(BL)];
    Vector3 &vBR = m_terrain[getIndex(BR)];

    //Grid coord points
    float xwide = abs(botright.x - topleft.x); // width of the big rectangle
    float ywide = abs(topleft.y - botright.y);

    //Grid coord points
    float tmx = TL.x + xwide/2;
    float bmx = BL.x + xwide/2;
    float mlx = TL.x;
    float mrx = BR.x;
    float mmx = BL.x + xwide/2;
    float tmy = TL.y;
    float bmy = BL.y;
    float mly = TL.y + ywide/2;
    float mry = TL.y + ywide/2;
    float mmy = TL.y + ywide/2;

    GridPoint TM = GridPoint(tmx, tmy);
    GridPoint MM = GridPoint(mmx, mmy);
    GridPoint BM = GridPoint(bmx, bmy);
    GridPoint ML = GridPoint(mlx, mly);
    GridPoint MR = GridPoint(mrx, mry);

    Vector3 mm = (vTL + vTR + vBL + vBR)/4;
    mm.y += getPerturb(curDepth);

    m_terrain[getIndex(TM)] = (vTL + vTR)/2;
    m_terrain[getIndex(BM)] = (vBL + vBR)/2;
    m_terrain[getIndex(MM)] = mm;
    m_terrain[getIndex(ML)] = (vTL + vBL)/2;
    m_terrain[getIndex(MR)] = (vTR + vBR)/2;

    subdivideSquare( TL, MM,curDepth -1 );
    subdivideSquare( TM, MR,curDepth -1 );
    subdivideSquare( ML, BM,curDepth -1 );
    subdivideSquare( MM, BR,curDepth -1 );

    //
    // @TODO: [Lab 5] Above, we demonstrate how to find
    //                  a) the coordinates in the grid of the corner vertices (in grid space)
    //                  b) the actual (X,Y,Z) points in the terrain map
    //
    //         Now, you just need to compute the midpoints in grid space, average the
    //         heights of the neighboring vertices, and write the new points to the
    //         terrain map. Remember to perturb the new center vertex by a random amount.
    //         Scale this perturbation by curDepth so that the perturbation factor is less
    //         as you recur deeper.
    //
    //         Remember that [X,Y] grid points map to [X,Z] in world space -- the Y
    //         coordinate in world space will be used for the height.
    //
    //         Once you compute these remaining 5 points, you will need to recur on the
    //         four sub-squares you've just created. To do this, you'll call recurOnSquare
    //         recursively, decrementing curDepth by one.
    //
}

/**
 * You need to fill this in.
 *
 * Computes the normal vector of each terrain vertex and stores it in the corresponding vertex.
 */
void Lab5Canvas::computeNormals()
{
    // For each vertex in the 2D grid...
    for (int row = 0; row < m_gridLength; row++)
    {
        for (int column = 0; column < m_gridLength; column++)
        {
            const GridPoint gridPosition(row, column);                // 2D coordinate of the vertex on the terrain grid
            const int terrainIndex = getIndex(gridPosition);          // Index into the 1D position and normal arrays
            const Vector3& vertexPosition  = m_terrain[terrainIndex]; // Position of the vertex

            // Get the neighbors of the vertex at (a,b)
            const QList<Vector3*>& neighbors = getSurroundingVertices(gridPosition);
            int numNeighbors = neighbors.size();

            // @TODO: [Lab 5] Compute a list of vectors from vertexPosition to each neighbor in neighbors
            Vector3 *offsets = new Vector3[numNeighbors];
            for (int i = 0; i < numNeighbors; ++i)
            {
                offsets[i] = Vector3::zero();
                offsets[i] = *neighbors.at(i)- (vertexPosition);
            }

            // @TODO: [Lab 5] Compute cross products for each neighbor
            Vector3 *normals = new Vector3[numNeighbors];
            for (int i = 0; i < numNeighbors; ++i)
            {
                normals[i] = Vector3::zero();
                normals[i] = offsets[i].cross(offsets[(i+1) % numNeighbors]);
            }


            // Average the normals and store the final value in the normal map
            Vector3 sum = Vector3::zero();
            for (int i = 0; i < numNeighbors; ++i)
                sum += normals[i];
            m_normalMap[terrainIndex] = sum.getNormalized();

            delete[] offsets;
            delete[] normals;
        }
    }
}

/**
 * You need to fill this in.
 *
 * Renders the terrain to the canvas. This is where you should make all of your OpenGL calls.
 */
void Lab5Canvas::paintGL()
{
    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Push a new matrix onto the stack for modelling transformations
    glPushMatrix();

    // @TODO [Lab5] Tesselate your terrain here.

    for(int i = 0; i< m_gridLength - 1; i++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for(int j=0; j<m_gridLength; j++)
        {
            glNormal3dv(m_normalMap[getIndex(i,j)].data);
            glTexCoord2f((float)i/(float)m_gridLength, (float)j/(float)m_gridLength);
            glVertex3dv(m_terrain[getIndex(i,j)].data);
            glTexCoord2f((float)(i+1)/(float)m_gridLength, (float)j/(float)m_gridLength);
            glVertex3dv(m_terrain[getIndex(i+1,j)].data);
        }
        glEnd();
    }

    //draw normals
    drawNormals();
    // Discard the modelling transformations (leaving only camera settings)
    glPopMatrix();
    // Force OpenGL to perform all pending operations -- usually a good idea to call this
    glFlush();
    // Swap the buffers to show what we have just drawn onto the screen
    swapBuffers();
}

/**
 * Computes the amount to perturb the height of the vertex currently being processed.
 * Feel free to modify this.
 *
 * @param depth The current recursion depth
 */
double Lab5Canvas::getPerturb(int cur_depth)
{
    return m_roughness
           * pow((double)cur_depth / m_depth, m_decay)
           * ((rand() % 200-100) / 100.0);
}

/****************************************************************************************************************/
/**********************************DO NOT MODIFY ANY OF THE FOLLOWING CODE***************************************/
/****************************************************************************************************************/
/**
 * Sets default values for the four corners of the terrain grid and calls subdivideSquare()
 * to begin the terrain generation process. You do not need to modify this function.
 */
void Lab5Canvas::populateTerrain()
{
    Vector3 tl(-10, 0, -10);
    Vector3 tr(10, 0, -10);
    Vector3 bl(-10, 0, 10);
    Vector3 br(10, 0, 10);
    GridPoint tlg(0,0);
    GridPoint trg(0,m_gridLength-1);
    GridPoint blg(m_gridLength-1, 0);
    GridPoint brg(m_gridLength-1, m_gridLength-1);
    m_terrain[getIndex(tlg)] = tl;
    m_terrain[getIndex(trg)] = tr;
    m_terrain[getIndex(blg)] = bl;
    m_terrain[getIndex(brg)] = br;
    subdivideSquare(tlg, brg, m_depth);
}

/**
  * Converts a grid coordinate (row, column) to an index into a 1-dimensional array.
  * Can be used to index into m_terrain or m_normalMap.
  * Returns -1 if the grid coordinate entered is not valid.
  */
inline int Lab5Canvas::getIndex(const GridPoint &c)
{
    return getIndex(c.x, c.y);
}

/**
  * Converts a grid coordinate (row, column) to an index into a 1-dimensional array.
  * Can be used to index into m_terrain or m_normalMap.
  * Returns -1 if the grid coordinate entered is not valid.
  */
inline int Lab5Canvas::getIndex(int row, int col)
{
    if (row < 0 || row >= m_gridLength || col < 0 || col >= m_gridLength)
        return -1;

    return row * m_gridLength + col;
}

// All of the following methods are to be used in finding the normals

/**
 * Retrieves the position of each neighbor of the given grid coordinate (i.e. all grid
 * coordinates that can be found one unit horizontally, vertically, or diagonally from
 * the specified grid coordinate).
 *
 * @param coordinate The grid coordinate whose neighbors are to be retrieved
 */
QList<Vector3*> Lab5Canvas::getSurroundingVertices(const GridPoint &coordinate)
{
    GridPoint coords[8];
    coords[0] = GridPoint(coordinate.x,     coordinate.y - 1);
    coords[1] = GridPoint(coordinate.x + 1, coordinate.y - 1);
    coords[2] = GridPoint(coordinate.x + 1, coordinate.y);
    coords[3] = GridPoint(coordinate.x + 1, coordinate.y + 1);
    coords[4] = GridPoint(coordinate.x,     coordinate.y + 1);
    coords[5] = GridPoint(coordinate.x - 1, coordinate.y + 1);
    coords[6] = GridPoint(coordinate.x - 1, coordinate.y);
    coords[7] = GridPoint(coordinate.x - 1, coordinate.y - 1);

    int index;
    QList<Vector3*> vecs;

    for (int i = 0; i < 8; i++)
    {
        index = getIndex(coords[i]);
        if (index != -1)
            vecs.push_back(& m_terrain[index]);
    }

    return vecs;
}

/**
 * Draws a line at each vertex showing the direction of that vertex's normal. You may find
 * this to be a useful tool if you're having trouble getting the lighting to look right.
 * By default, this function is called in paintGL(), but only renders anything if
 * m_renderNormals is true. You do not need to modify this function.
 */
void Lab5Canvas::drawNormals()
{
    if (m_renderNormals)
    {
        glColor3f(1,0,0);

        for (int row = 0; row < m_gridLength; row++)
        {
            for (int column = 0; column < m_gridLength; column++)
            {
                glBegin(GL_LINES);

                Vector3 curVert = m_terrain[getIndex(row, column)];
                Vector3 curNorm = m_normalMap[getIndex(row, column)];

                glNormal3dv(curNorm.data);
                glVertex3f(curVert.x, curVert.y, curVert.z);
                glVertex3f(curVert.x +curNorm.x,
                           curVert.y + curNorm.y,
                           curVert.z + curNorm.z);

                glEnd();
            }
        }
    }
}

/** Sets the initial values of OpenGL state variables used by this lab */
void Lab5Canvas::initializeGL()
{
    glClearColor(0, 0, 0, 0);   // Always reset the screen to black before drawing anything
    glEnable(GL_DEPTH_TEST);    // When drawing a triangle, only keep pixels closer to the camera than what's already been drawn

    // Make things pretty
    glEnable(GL_MULTISAMPLE);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // Bind the ambient and diffuse color of each vertex to the current glColor() value
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Cull triangles that are facing away from the camera
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set up a single light
    glEnable(GL_LIGHTING);
    GLfloat ambientColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat diffuseColor[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat specularColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat lightPosition[] = { 0.f, 0.f, 10.f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT0);

    // Enable texturing - feel free to substitue with your own image
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    if (loadTexture("/course/cs123/data/image/BoneHead.jpg") == -1)
        cout << "Texture does not exist" << endl;

    populateTerrain();
    computeNormals();
}

/** Updates the current OpenGL state to avoid object distortion when the window is resized. */
void Lab5Canvas::resizeGL(int w, int h)
{
    // Prevent a divide by zero
    if (w < 1) w = 1;
    if (h < 1) h = 1;

    // Set the viewport to be the entire window (you probably want to keep this)
    glViewport(0, 0, w, h);
    updateCamera();
}

/**
 * Called whenever m_camera is modified or the canvas is resized. Sets the current OpenGL projection
 * and modelview matrices to match the values in m_camera.
 */
void Lab5Canvas::updateCamera()
{
    float w = width(), h = height();
    float ratio = 1.0f * w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera->angle, ratio, m_camera->near, m_camera->far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_camera->eye.x, m_camera->eye.y, m_camera->eye.z,
              m_camera->center.x, m_camera->center.y, m_camera->center.z,
              m_camera->up.x, m_camera->up.y, m_camera->up.z);
}

void Lab5Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
        m_lastMousePos.x = event->x(); m_lastMousePos.y = event->y();
}

void Lab5Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    {
        emit _mouseDrag(event);
        m_lastMousePos.x = event->x(); m_lastMousePos.y = event->y();
    }
}

void Lab5Canvas::mouseDragEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton || event->buttons() & Qt::LeftButton)
    {
        float dx = event->x() - m_lastMousePos.x,
              dy = event->y() - m_lastMousePos.y;

        double x = m_camera->eye.x, y = m_camera->eye.y, z = m_camera->eye.z,
               r = sqrt(m_camera->eye.x * m_camera->eye.x +
                        m_camera->eye.y * m_camera->eye.y +
                        m_camera->eye.z * m_camera->eye.z),
               theta = acos(y / r) - dy * 0.01f,
               phi = atan2(z, x) + dx * 0.01f;

        if (theta > M_PI-.1) theta = M_PI - .1;
        if (theta < .1) theta = .1;

        m_camera->eye.x = r * sin(theta) * cos(phi);
        m_camera->eye.y = r * cos(theta);
        m_camera->eye.z = r * sin(theta) * sin(phi);

        updateCamera();
    }
}

void Lab5Canvas::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Q)
        exit(0);
}

void Lab5Canvas::redraw()
{
    repaint();
}

void Lab5Canvas::settingsChanged() { }

/**
 * Generates a new OpenGL texture and loads the image data from the file at the given path into
 * the texture. Makes the new texture active and returns its unique texture ID.
 */
GLuint Lab5Canvas::loadTexture(const QString &filename)
{
    // Make sure the image file exists
    QFile file(filename);
    if (!file.exists())
        return -1;

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
