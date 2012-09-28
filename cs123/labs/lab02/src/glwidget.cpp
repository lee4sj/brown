#include "glwidget.h"
#include "settings.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent), m_timer(this), m_fps(60.0f), m_increment(0), m_angleX(0.0f), m_angleY(0.0f), m_xDiff(0.0f), m_zDiff(0.0f), m_arrowRadius(0.1), m_targetRadius(0.5), m_score(0), m_canCollide(false)
{

    // Set up the camera
    m_camera.eye.x = 0.0f, m_camera.eye.y = 0.0f, m_camera.eye.z = -1.0f;
    m_camera.center.x = 0.0f, m_camera.center.y = 0.0f, m_camera.center.z = 0.0f;
    m_camera.up.x = 0.0f, m_camera.up.y = 1.0f, m_camera.up.z = 0.0f;
    m_camera.fovy = 45.0f, m_camera.near = 1.0f, m_camera.far = 1000.0f;

    // Set up 60 FPS draw loop
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));

    this->setMouseTracking(true);
    m_originalMouseX = -1;
    m_originalMouseY = -1;

    m_arrowPos = Vector3(0,0,0);
    m_targetPos = m_arrowPos;
}

GLWidget::~GLWidget()
{
    gluDeleteQuadric(m_quadric);
}

/**
  Renders the target and sets its position and size via m_targetPos and m_targetRadius
  You have to fill in this method
  **/
void GLWidget::renderTarget()
{
    //TODO: implement this method

}

/**
  Method to render the intersection sphere for the target
  You need to fill in this method
  **/
void GLWidget::renderTargetSphere()
{
    //TODO: implement this method

}

/**
  Method to render the intersection sphere for the arrow
  You need to fill in this method
  **/
void GLWidget::renderArrowSphere()
{
    //TODO: implement this method

}

/**
  Called once at GL initialization time to setup OpenGL states
**/
void GLWidget::initializeGL()
{
    // Get a new quadrics obect
    m_quadric = gluNewQuadric();

    // Enable Lighting
    glEnable(GL_LIGHTING);

    // Enable depth testing, so that objects are occluded based on depth instead of drawing order
    glEnable(GL_DEPTH_TEST);

    // Enable back-face culling, meaning only the front side of every face is rendered
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Specify that the front face is represented by vertices in counterclockwise order (this is the default)
    glFrontFace(GL_CCW);

    // Enable color materials with ambient and diffuse lighting terms
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Set up global (ambient) lighting
    GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    // Set up GL_LIGHT0 with a position and lighting properties
    GLfloat ambientLight[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
    GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 2.0f, 2.0f, 2.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // Set the screen color when the color buffer is cleared (in RGBA format)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable light 0
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    // Load the initial settings
    updateSettings();
    updateCamera();

}

/**
  The main drawing method which will be called 30 frames per second.  Here
  is where your OpenGL drawing should occur.
**/
void GLWidget::paintGL()
{
    // Get the time
    float time = 5*m_increment / (float) m_fps;

    //if we haven't fired yet, update the angles so that the arrow's angles and position match the cameras
    if(!m_fired)
    {
        m_firedAngleX = m_angleX;
        m_firedAngleY = m_angleY;
        m_firedXDiff = m_xDiff;
        m_firedZDiff = m_zDiff;
    }

    // Clear the color and depth buffers to the current glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0f, .5f, 0.0f);
    renderTarget();
    glPushMatrix();

    //transform to get to camera coordinates to render the arrow
    glTranslatef(-m_xDiff, 0.0f, -m_zDiff);
    glRotatef(m_angleY, cos(M_PI*m_angleX/180), 0.0f, sin(M_PI*m_angleX/180));
    glRotatef(-m_angleX, 0.0f, 1.0f, 0.0f);

    glRotatef(15, 0.0f, 1.0f, 0.0f);

    glTranslatef(-0.8f, 0.5f, -2.5f);
    glScalef(2.5f, 2.5f, 2.5f);
    renderBow();

    glPopMatrix();
    glPushMatrix();

    //start arrow off at fired position i.e. camera position when the arrow was fired or current camera position if it hasn't been fired
    m_arrowPos =  Vector3(-m_firedXDiff, 0.0f, -m_firedZDiff);
    //set the velocity to reflect the rotation transforms we do to render the arrow
    //we keep variables for all the values we need for this to simplify the velocity vector
    double cx = cos(-m_firedAngleX * M_PI/180);
    double sx = sin(-m_firedAngleX * M_PI/180);
    double cy = cos(m_firedAngleY * M_PI/180);
    double omcy = 1.0-cy;
    double sy = sin(m_firedAngleY * M_PI/180);

    double cmx = cos(M_PI*m_firedAngleX/180);
    double smx = sin(M_PI*m_firedAngleX/180);

    //calculates through the rotates done to get an accurate velocity vector
    Vector3 vel = Vector3((sx*((cmx*cmx*omcy) + cy)) + (cx*cmx*smx*omcy),
                          (sx*smx*sy) - (cx*cmx*sy),
                          (sx*cmx*smx*omcy) + (cx*((smx*smx*omcy) + cy)));

    //move the arrow's position based on velocity and time
    m_arrowPos += time*vel;

    //look for a hit, and if we find one, stop the arrow
    if(m_canCollide && (m_arrowPos - m_targetPos).length() < m_arrowRadius + m_targetRadius && m_timer.isActive())
    {
        m_scoreLabel->setText("Score: " + QString::number(++m_score));
        m_timer.stop();
    }

    //transform to get to camera coordinates to render the arrow
    glTranslatef(-m_firedXDiff, 0.0f, -m_firedZDiff);
    glRotatef(m_firedAngleY, cmx, 0.0f, smx);
    glRotatef(-m_firedAngleX, 0.0f, 1.0f, 0.0f);
    glTranslatef(-0.5, 0, 1.0 + time);
    if(!m_fired)
    {
        glRotatef(30, 0.0f, 1.0f, 0.0f);
    }
    //move and rotate to make the arrow face straight
    else
    {
        glTranslatef(qMin(0.5f*time, 0.5f), 0.0f, qMax(-0.7f, -1.0f*time));
        glRotatef(qMax(15.0f-(time*30), 0.5f), 0.0f, 1.0f, 0.0f);
    }

    renderArrow();
    glPopMatrix();

    //render the walls, floor and ceiling of our playing field
    glColor3f(0.0f, 0.7f, 0.93f);
    glPushMatrix();
    glTranslatef(-5.0f, -1.0f, 5.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    renderQuad();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5.0f, -1.0f, -3.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    glRotatef(90, 0.0f, 1.0f, 0.0f);
    renderQuad();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5.0f, 9.0f, -3.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    renderQuad();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5.0f, 9.0f, -3.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    renderQuad();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5.0f, 5.0f, -3.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    renderQuad();
    glPopMatrix();

    glColor3f(0.3f, 0.74f, 0.2f);
    glPushMatrix();
    glTranslatef(5.0f, -1.0f, -3.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    glRotatef(90, 0.0f, 0.0f, 1.0f);
    renderQuad();
    glPopMatrix();

    //Render intersection spheres
    if(m_canCollide && settings.showIntersectSpheres)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(0.5f, 0.5f, 0.5f);

        renderTargetSphere();

        renderArrowSphere();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}

/**
  This method renders an arrow created via composition of glu objects
  **/
void GLWidget::renderArrow()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    gluCylinder(m_quadric, 0.05f, 0.0f, 0.1f, 10, 10);
    glPushMatrix();
    glColor3f(0.37f, 0.15f, 0.02f);
    glTranslatef(0.0f, 0.0f, -0.96f);
    gluCylinder(m_quadric, 0.025f, 0.025f, 1.0f, 10, 10);
    glColor3f(0.0f, 0.7f, 0.0f);
    gluSphere(m_quadric, 0.03f, 10, 10);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.02f, -0.05f);
    glScalef(1.0f, 0.05f, 0.1f);
    renderQuad();
    glPopMatrix();
    glPushMatrix();
    glRotatef(120, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.02f, -0.05f);
    glScalef(1.0f, 0.05f, 0.1f);
    renderQuad();
    glPopMatrix();
    glPushMatrix();
    glRotatef(240, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.02f, -0.05f);
    glScalef(1.0f, 0.05f, 0.1f);
    renderQuad();
    glPopMatrix();
    glPopMatrix();
}

/**
  Renders the bow object
  **/
void GLWidget::renderBow(){
    glColor3f(0.37f, 0.15f, 0.02f);
    glTranslatef(0.0, 0.0, 1.0f);
    glPushMatrix();
    glScalef(.01f, 0.5f, .01f);

    glRotatef(90, 1.0f, 0.0f, 0.0f);
    gluCylinder(m_quadric, 1.0f, 1.0f, 1.0f, 10, 10);
    glPopMatrix();

    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, -0.125f, 0.21f);
    glScalef(.01f, 0.5f, .01f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    gluCylinder(m_quadric, 1.0f, 1.0f, 0.5f, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glRotatef(30, 1.0f, 0.0f, 0.0f);
    glScalef(.01f, 0.01f, .5f);
    gluCylinder(m_quadric, 1.0f, 1.0f, 0.5f, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, -0.5f, 0.0f);
    glRotatef(-30, 1.0f, 0.0f, 0.0f);
    glScalef(.01f, 0.01f, .5f);
    gluCylinder(m_quadric, 1.0f, 1.0f, 0.5f, 10, 10);
    glPopMatrix();
}

/**
  renders a quad that is visible from both front and back
  **/
void GLWidget::renderQuad()
{
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 1.0f);

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);


    glEnd();
}

/**
  Called when the screen gets resized.
  The camera is updated when the screen resizes because the aspect ratio may change.
**/
void GLWidget::resizeGL(int width, int height)
{
    // Set the viewport to fill the screen
    glViewport(0, 0, width, height);
    // Update the camera
    updateCamera();
}

/**
  Update the camera's specifications.
  It gets called in resizeGL which get called automatically on intialization
  and whenever the window is resized.
**/
void GLWidget::updateCamera()
{
    float w = width();
    float h = height();
    float aspectRatio = 1.0f * w / h;
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera.fovy, aspectRatio, m_camera.near, m_camera.far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(m_camera.eye.x, m_camera.eye.y, m_camera.eye.z,
              m_camera.center.x, m_camera.center.y, m_camera.center.z,
              m_camera.up.x, m_camera.up.y, m_camera.up.z);
    //rotate and translate for fps controls
    glRotatef(m_angleX, 0.0f, 1.0f, 0.0f);
    glRotatef(-m_angleY, cos(M_PI*m_angleX/180), 0.0f, sin(M_PI*m_angleX/180));
    glTranslatef(m_xDiff, 0.0f, m_zDiff);
}

/**
  This method handles key press events to allow for movement
  **/
void GLWidget::keyPressEvent ( QKeyEvent * event )
{
    //we adjust how we move by what angle we're currently facing
    double cx = cos(-m_angleX * M_PI/180);
    double sx = sin(-m_angleX * M_PI/180);
    if(event->key() == Qt::Key_W)
    {
        m_zDiff -= 0.025f * cx;
        m_xDiff -= 0.025f * sx;
        this->updateCamera();
        this->update();
    }
    else if(event->key() == Qt::Key_S)
    {
        m_zDiff += 0.025f * cx;
        m_xDiff += 0.025f * sx;
        this->updateCamera();
        this->update();
    }
    else if(event->key() == Qt::Key_D)
    {
        m_zDiff += 0.025f * -sx;
        m_xDiff += 0.025f * cx;
        this->updateCamera();
        this->update();
    }
    else if(event->key() == Qt::Key_A)
    {
        m_zDiff -= 0.025f * -sx;
        m_xDiff -= 0.025f * cx;
        this->updateCamera();
        this->update();
    }
    else if(event->key() == Qt::Key_Escape)
    {
        m_firstPersonMode = false;
        m_originalMouseX = -1;
        m_originalMouseY = -1;
    }
}

/**
  This method handles mouse movement events for camera rotation
  **/
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_firstPersonMode) {
        if(m_originalMouseX < 0 && m_originalMouseY < 0)
        {
            m_originalMouseX = event->globalX() - event->x() + this->width()/2;
            m_originalMouseY = event->globalY() - event->y() + this->height()/2;
            QCursor::setPos(m_originalMouseX, m_originalMouseY);
        }
        else
        {
            float x = event->globalX();
            float y = event->globalY();
            rotateCamera(x - m_originalMouseX, y-m_originalMouseY);
            update();
            QCursor::setPos(m_originalMouseX, m_originalMouseY);
        }
        update();
    }
}

/**
  This method handles click events. It enters first person mode if the user isn't in first person mode,
  shoots an arrow if the user has an arrow to shoot and picks up an arrow if the user has none
  **/
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(!m_firstPersonMode){
        m_firstPersonMode = true;
        m_originalMouseX = event->globalX() - event->x() + this->width()/2;
        m_originalMouseY = event->globalY() - event->y() + this->height()/2;
        QCursor::setPos(m_originalMouseX, m_originalMouseY);
    }
    else {
        m_originalMouseX = event->globalX();
        m_originalMouseY = event->globalY();
        if(m_increment == 0.0f && !m_timer.isActive())
        {
            //start the timer if the increment is 0
            m_timer.start(1000.0f / m_fps);
            m_fired = true;
        }
        else
        {
            //reset the timer
            m_increment = 0.0f;
            m_fired = false;
            m_timer.stop();
        }
        update();
    }

}

/**
  Modifies the angle that the user is looking via deltaX and deltaY
  **/
void GLWidget::rotateCamera(float deltaX, float deltaY)
{
    m_angleX +=  deltaX * 0.025;
    m_angleY +=  deltaY * 0.025;
    m_angleY = qMax(-90 + 0.001, qMin(90 - 0.001, (double)m_angleY));
    updateCamera();
}

/**
  Called when the settings are updated through the UI.
  This is already filled in for you.
**/
void GLWidget::updateSettings()
{
    update();
}

/**
  Allows the ui to set the label to be modified to update score
  **/
void GLWidget::setLabel(QLabel* label)
{
    m_scoreLabel = label;
}

/**
  Sets the position of the target, and notifies the system that we are ready to attempt collisions
  **/
void GLWidget::setTargetPosition(Vector3 pos)
{
    m_canCollide = true;
    m_targetPos = pos;
}

/**
  Specifies to Qt what to do when the widget needs to be updated.
  We only want to repaint the onscreen objects.
**/
void GLWidget::tick()
{
    m_increment++;
    update();
    if(m_increment/(float) m_fps > 1.0f)
    {
        //reset the timer and set fired to false
        m_timer.stop();
        m_increment = 0.0f;
        m_fired = false;
    }
}
