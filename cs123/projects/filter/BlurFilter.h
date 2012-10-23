#ifndef BLURFILTER_H
#define BLURFILTER_H

#include "filter/Filter.h"

class Canvas2D;

struct BGRADouble {
    double r;
    double g;
    double b;
};

class BlurFilter : public Filter
{
public:
    BlurFilter();
    virtual ~BlurFilter();
    virtual void applyFilter(Canvas2D *canvas, int radius);

protected:
    void getGaussian(double *kernel, int radius);
    void applyHorizontalKernel(BGRADouble *convIm, BGRA *image, int width,
                               double *kernel, QPoint &start, QPoint &end, int r);
    void applyVerticalKernel(BGRA *convIm, BGRADouble *image, int width,
                             double *kernel, QPoint &start, QPoint &end, int r);
};

#endif // BLURFILTER_H
