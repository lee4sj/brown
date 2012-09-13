/*!
   @file   LinearBrush.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef LinearBrush_Header
#define LinearBrush_Header

#include "Brush.h"

//! This brush should have a mask with linear falloff.
class LinearBrush : public Brush {

    public:

        LinearBrush(BGRA color, int flow, int radius);
    virtual ~LinearBrush();

    protected:
        void makeMask(); /// Constructs the mask for this brush.
};

#endif
