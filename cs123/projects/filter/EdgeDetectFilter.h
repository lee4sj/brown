#ifndef EDGEDETECTFILTER_H
#define EDGEDETECTFILTER_H

#include "filter/GreyscaleFilter.h"

class Canvas2D;

class EdgeDetectFilter : public GreyscaleFilter
{
public:
    EdgeDetectFilter();
    virtual ~EdgeDetectFilter();
    virtual void applyFilter(Canvas2D *canvas, double filterSensitivity);

private:
    void computeGx(BGRA *image, float *convIm, QPoint &start, QPoint &end, int width);
    void computeGy(BGRA *image, float *convIm, QPoint &start, QPoint &end, int width);
};

#endif // EDGEDETECTFILTER_H
