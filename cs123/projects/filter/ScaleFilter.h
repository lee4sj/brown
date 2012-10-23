#ifndef SCALEFILTER_H
#define SCALEFILTER_H

#include "filter/BlurFilter.h"

class ScaleFilter : public BlurFilter
{
public:
    ScaleFilter();
    virtual ~ScaleFilter();
    virtual void applyFilter(Canvas2D *canvas, double scaleX, double scaleY);

//private:
//    virtual void scaleDown(Canvas2D *canvas, BGRA *im, int width, int height,
//                           int newWidth, int newHeight);
};

#endif // SCALEFILTER_H
