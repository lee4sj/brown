/*!
   @file   RayScene.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "SceneviewScene.h"


/*!
  @class RayScene
  @brief Students will implement this class as necessary in the Ray project.
**/
class RayScene : public SceneviewScene
{
public:
    RayScene();
    virtual ~RayScene();

    void trace(BGRA *data, Camera *camera, int width, int height);

private:
    void findEyeRays(QList<Vector4> &eyeRays, int width, int height, Camera *camera);
};

#endif // RAYSCENE_H
