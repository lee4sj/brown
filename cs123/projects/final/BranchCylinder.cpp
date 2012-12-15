#include "BranchCylinder.h"
#include "CS123Algebra.h"
#include "OpenGLScene.h"

#include <qgl.h>


BranchCylinder::BranchCylinder()
{
}

BranchCylinder::~BranchCylinder()
{
}

void BranchCylinder::renderGeometry(int param1, int param2, float param3)
{
    renderGeometry(1.0, 0.5, 0.3);
}

void BranchCylinder::renderGeometry(float length, double rStart, double rEnd)
{
//    int param1 = 10;
//    int param2 = 10;

//    double yStep = length/(double)param1;
//    double thetaStep = 2*M_PI/((double)param2 + 2);

//    double x;
//    double z;
//    double xNext;
//    double zNext;
//    double normConst = 1/r;

//    glBegin(GL_TRIANGLES);
//    for (double theta = 0; theta < 2*M_PI; theta += thetaStep) {
//        float xRand = (float)rand()/((float)RAND_MAX) * 0.01 - 0.005;
//        float zRand = (float)rand()/((float)RAND_MAX) * 0.01 - 0.005;

//        x = xNext;//(r) * cos(theta) + xRand;
//        z = zNext;//(r) * sin(theta) + zRand;
//        xNext = r * cos(theta + thetaStep) + xRand;
//        zNext = r * sin(theta + thetaStep) + zRand;

//        //render sides
//        for (double y = 0; y <= length - yStep/2; y += yStep) {

//            glNormal3f(normConst * x, 0, normConst * z);
//            glVertex3f(x, y, z);
//            glNormal3f(normConst * xNext, 0, normConst * zNext);
//            glVertex3f(xNext, y + yStep,zNext);
//            glNormal3f(normConst * xNext, 0, normConst * zNext);
//            glVertex3f(xNext, y, zNext);


//            glNormal3f(normConst * x, 0, normConst * z);
//            glVertex3f(x, y, z);
//            glNormal3f(normConst * x, 0, normConst * z);
//            glVertex3f(x, y + yStep, z);
//            glNormal3f(normConst * xNext, 0, normConst * zNext);
//            glVertex3f(xNext, y + yStep, zNext);
//        }

//        //render the bottom and the top
//        glNormal3f(0, 1, 0);
//        glVertex3f(0, length, 0);
//        glVertex3f(xNext, length, zNext);
//        glVertex3f(x, length, z);

//        glNormal3f(0, -1, 0);
//        glVertex3f(0, 0, 0);
//        glVertex3f(x, 0, z);
//        glVertex3f(xNext, 0, zNext);
//    }
//    glEnd();

    int param1 = max(1, 50 * length);
    int param2 = 10;

    double rStep = (rStart - rEnd)/(double)param1;
    double yStep = length/(double)param1;
    double thetaStep = 2*M_PI/((double)param2 + 2);

    double totalLength = length * (rStart/ rEnd);
    Vector3 normal(0.0,totalLength,0.0);

    double x;
//    double y = -0.5;
    double z;
    double xNext;
    double zNext;
    double xRNext;
    double zRNext;
    double xNextRNext;
    double zNextRNext;
    double normX;
    double normY = 1.0/(2 * sqrt(5.0/4));
    double normZ;
    double normXNext;
    double normZNext;

    double r = rStart;

    glBegin(GL_TRIANGLES);
    for (double y = 0; y <= length - yStep/2; y += yStep) {
        for (double theta = 2*M_PI; theta > 0; theta -= thetaStep) {
            x = r * cos(theta);
            z = r * sin(theta);
            xNext = r * cos(theta - thetaStep);
            zNext = r * sin(theta - thetaStep);

            xRNext = (r - rStep) * cos(theta);
            zRNext = (r - rStep) * sin(theta);
            xNextRNext = (r - rStep) * cos(theta - thetaStep);
            zNextRNext = (r - rStep) * sin(theta - thetaStep);

//            normal.x = cos(theta)*rStart;
//            normal.z = sin(theta)*rStart;
//            normal.normalize();
//            normX = normal.x;
//            normZ = normal.z;
//            normal = Vector3(cos(theta - thetaStep)*rStart, totalLength, sin(theta - thetaStep)*rStart);
//            normal.normalize();
//            normXNext = normal.x;
//            normZNext = normal.z;

//            normX = cos(theta)/sqrt(5.0/4);
//            normZ = sin(theta)/sqrt(5.0/4);
//            normXNext = cos(theta - thetaStep)/sqrt(5.0/4);
//            normZNext = sin(theta - thetaStep)/sqrt(5.0/4);

            normX = cos(theta);
            normZ = sin(theta);
            normXNext = cos(theta-thetaStep);
            normZNext = sin(theta-thetaStep);
            normY = 0.0;

            glNormal3f(normX, normY, normZ);
            glTexCoord2f(theta / (2*M_PI), y / length);
            glVertex3f(x, y, z);

            glNormal3f(normXNext, normY, normZNext);
            glTexCoord2f((theta-thetaStep) / (2*M_PI), y / length);
            glVertex3f(xNext, y, zNext);

            glNormal3f(normXNext, normY, normZNext);
            glTexCoord2f((theta-thetaStep) / (2*M_PI), (y + yStep) / length);
            glVertex3f(xNextRNext, y + yStep, zNextRNext);


            glNormal3f(normX, normY, normZ);
            glTexCoord2f(theta / (2*M_PI), y / length);
            glVertex3f(x, y, z);

            glNormal3f(normXNext, normY, normZNext);
            glTexCoord2f((theta-thetaStep) / (2*M_PI), (y + yStep) / length);
            glVertex3f(xNextRNext, y + yStep, zNextRNext);

            glNormal3f(normX, normY, normZ);
            glTexCoord2f(theta / (2*M_PI), (y  + yStep) / length);
            glVertex3f(xRNext, y + yStep, zRNext);
        }
        r -= rStep;
    }

//    //draw the tip
//    for (double theta = 2*M_PI; theta > 0; theta -= thetaStep) {
//        x = r * cos(theta);
//        z = r * sin(theta);
//        xNext = r * cos(theta - thetaStep);
//        zNext = r * sin(theta - thetaStep);

//        normX = cos(theta)/sqrt(5.0/4);
//        normZ = sin(theta)/sqrt(5.0/4);
//        normXNext = cos(theta - thetaStep)/sqrt(5.0/4);
//        normZNext = sin(theta - thetaStep)/sqrt(5.0/4);

//        glNormal3f(normX, normY, normZ);
//        glVertex3f(x, y, z);
//        glNormal3f(normXNext, normY, normZNext);
//        glVertex3f(xNext, y, zNext);
//        glNormal3f(normXNext, normY, normZNext);
//        glVertex3f(0, y + yStep, 0);

//        glNormal3f(normX, normY, normZ);
//        glVertex3f(x, y, z);
//        glNormal3f(normXNext, normY, normZNext);
//        glVertex3f(0, y + yStep, 0);
//        glNormal3f(normX, normY, normZ);
//        glVertex3f(0, y + yStep, 0);
//    }

    for (double theta = 2*M_PI; theta > 0; theta -= thetaStep) {
        glNormal3f(0, 1, 0);

        glTexCoord2f(rEnd * cos(theta) + rEnd, rEnd * sin(theta) + rEnd);
        glVertex3f(rEnd * cos(theta), length, rEnd * sin(theta));

        glTexCoord2f(rEnd * cos(theta - thetaStep) + rEnd, rEnd * sin(theta - thetaStep) + rEnd);
        glVertex3f(rEnd * cos(theta - thetaStep), length, rEnd * sin(theta - thetaStep));

        glTexCoord2f(0.5, 0.5);
        glVertex3f(0, length, 0);
    }

    //draw the bottom
    for (double theta = 2*M_PI; theta > 0; theta -= thetaStep) {
        glNormal3f(0, -1, 0);

        glTexCoord2f(0.0, 0.0);
        glVertex3f(0, 0, 0);

        glTexCoord2f(rStart * cos(theta - thetaStep) + rStart, rStart * sin(theta - thetaStep) + rStart);
        glVertex3f(rStart * cos(theta - thetaStep), 0, rStart * sin(theta - thetaStep));

        glTexCoord2f(rStart * cos(theta) + rStart, rStart * sin(theta) + rStart);
        glVertex3f(rStart * cos(theta), 0, rStart * sin(theta));

    }
    glEnd();

}



void BranchCylinder::renderNormals(float length, double rStart, double rEnd)
{
    int param1 = 10;
    int param2 = 10;

    double rStep = (rStart - rEnd)/(double)param1;
    double yStep = length/(double)param1;
    double thetaStep = 2*M_PI/((double)param2 + 2);

    double totalLength = length * (rStart/ rEnd) / (rStart/rEnd - 1);
    Vector4 normal(0.0,totalLength,0.0, 0.0);

    double x, z;
    double r = rStart;

    for (double y = 0; y <= length - yStep/2; y += yStep) {
        for (double theta = 2*M_PI; theta > 0; theta -= thetaStep) {
            x = r * cos(theta);
            z = r * sin(theta);

//            normal.x = cos(theta)*rStart;
//            normal.z = sin(theta)*rStart;
//            normal.normalize();

//            Vector4 curPos(x, y, z, 1.0);
//            renderNormal(curPos, normal);


            Vector4 normal = Vector4(cos(theta) / sqrt(5.0/4), 1.0/(2 * sqrt(5.0/4)), sin(theta) / sqrt(5.0/4), 1);
            Vector4 curPos(x, y, z, 1.0);
            renderNormal(curPos, normal);
        }
        r -= rStep;
    }
}

