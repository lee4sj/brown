#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include "camera.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void updateSettings();
    void updateCamera();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

protected slots:
    void tick();

private:
    GLUquadric *m_quadric;
    LabCamera m_camera;
    QTimer m_timer;
    float m_fps;
    float m_increment;
// Feel free to add your own member variables
};

#endif // GLWIDGET_H
