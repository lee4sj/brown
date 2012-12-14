#include "Shapes.h"
#include <qgl.h>

void Shapes::renderNormal(const Vector4 &vertex, const Vector4 &direction)
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

bool Shapes::isIntersect(Vector4 &eyePos, Vector4 &ray, Matrix4x4 invTrans, REAL &t, Vector4 &normal)
{
    return false;
}
