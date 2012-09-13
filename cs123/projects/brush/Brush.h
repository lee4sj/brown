/*!
   @file   Brush.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef __BRUSH_H__    //header guards
#define __BRUSH_H__

#include <CS123Common.h>


class Canvas2D;

/**

 @class Brush

 @brief This is the superclass for all brushes.

 Part of the CS123 support code.

 @author  Evan Wallace (edwallac)
 @author  Ben Herila (bherila)
 @date    9/1/2010

*/
class Brush {
public:

    //! Constructor for brush.  Pass it the initial color, flow and radius.
    Brush(BGRA color, int flow, int radius);  //contructor

    //! Destructor for brush.  Clean up all brush allocated resources.
    virtual ~Brush();                //destructor

    //! Set the red channel of the brush color.
    void setRed(int red);

    //! Set the green channel of the brush color.
    void setGreen(int green);

    //! Set the blue channel of the brush color.
    void setBlue(int blue);

    //! Set the flow of the brush.
    void setFlow(int flow);

    //! Set the radius of the brush.
    void setRadius(int radius);

    //! Apply the brush one time to the given canvas centered at (x,y)
    virtual void paintOnce(int x, int y, Canvas2D* canvas);


protected:
    /*! Pure virtual function that will create the mask distribution.
      Will be implemented in the subclass. */
    virtual void makeMask() = 0;
    unsigned char calcBlend(float mask, float alpha, float brush, float current);

    // C++ coding convention is to put m_ in front of member variables
    float* m_mask;  /// This is a pointer to the brush mask.
    BGRA m_color;   /// The color for the current brush.
    int m_flow;     /// The flow setting of the brush.
    int m_radius;   /// The radius setting of the brush.
};


#endif // __BRUSH_H__

