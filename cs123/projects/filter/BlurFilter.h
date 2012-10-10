#ifndef BLURFILTER_H
#define BLURFILTER_H

#include "filter/Filter.h"

class Canvas2D;

class BlurFilter : public Filter
{
public:
    BlurFilter();
    virtual ~BlurFilter();
    virtual void applyFilter(Canvas2D *canvas, int radius);

private:
    void getGaussian(double *kernel, int radius);
    void applyHorizontalKernel(BGRA *image, BGRA *convIm, double *kernel,
                               QPoint &start, QPoint &end, int width, int radius);
};

#endif // BLURFILTER_H
