/*!
   @file   ConstantBrush.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef ConstantBrush_Header
#define ConstantBrush_Header

#include "Brush.h"

//! This is a constant brush with equal mask distribution in a circle.
class ConstantBrush : public Brush {

    public:
        ConstantBrush(BGRA color, int flow, int radius);
    virtual ~ConstantBrush();

    protected:
        void makeMask(); /// Constructs the mask for this brush.
};

#endif
