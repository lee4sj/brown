#include "glwidget.h"
#include <iostream>

using namespace std;

static const int MAX_FPS = 120;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent),
        m_timer(this) {
    setFocusPolicy(Qt::StrongFocus);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
}

GLWidget::~GLWidget() {
    delete m_pong;
}

/**
    Called once at GL initialization time to setup OpenGL states
**/
void GLWidget::initializeGL() {
    m_timer.start(1000.0f / MAX_FPS);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    updateCamera();

    m_pong = new Pong(-1.0, 1.0, -1.0, 1.0);
}

/**
    This is called every time the screen is repainted.
**/
void GLWidget::paintGL() {
    m_pong->updateGame();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pong->drawObjects();
    paintScoreText();
}

/**
    This is called every time a key is pressed.
**/
void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_S:
            m_pong->setPaddleVelocity(-1, true);
            break;
        case Qt::Key_W:
            m_pong->setPaddleVelocity(1, true);
            break;
        case Qt::Key_Up:
            m_pong->setPaddleVelocity(1, false);
            break;
        case Qt::Key_Down:
            m_pong->setPaddleVelocity(-1, false);
            break;
        case Qt::Key_Space:
            m_pong->m_started = true;
            break;
    }
}

/**
    This is called every time a key is released.
    Stops paddle movement on keyup.
**/
void GLWidget::keyReleaseEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_S:
        case Qt::Key_W:
            m_pong->setPaddleVelocity(0, true);
            break;
        case Qt::Key_Up:
        case Qt::Key_Down:
            m_pong->setPaddleVelocity(0, false);
            break;
    }
}

void GLWidget::paintScoreText() {
    glColor3f(0.f, 0.f, 0.f);
    renderText(10, 35, QString::number(m_pong->getScore1()));
    renderText(this->width() - 20, 35, QString::number(m_pong->getScore2()));
}

/**
  Called when the screen gets resized.
  The camera is updated when the screen resizes because the aspect ratio may change.
**/
void GLWidget::resizeGL(int width, int height) {
    // Set the viewport to fill the screen
    glViewport(0, 0, width, height);
    // Update the camera
    updateCamera();
}

/**
  Update the camera's specifications.
  You need to fill in this method.
  It gets called in resizeGL which get called automatically on intialization
  and whenever the window is resized.
**/
void GLWidget::updateCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,0,1);
}

