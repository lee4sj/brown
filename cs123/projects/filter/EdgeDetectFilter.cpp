#include "EdgeDetectFilter.h"
#include "filter/GreyscaleFilter.h"

EdgeDetectFilter::EdgeDetectFilter() {}

EdgeDetectFilter::~EdgeDetectFilter() {}

void EdgeDetectFilter::computeGx(BGRA *image, float *convIm, QPoint &start, QPoint &end, int width)
{
    int widthConvIm = end.x() - start.x();

    //1. first convolve with [-1 0 1]
    for (int y = start.y(); y < end.y(); y++) {
        for (int x = start.x() + 1; x < end.x() - 1; x++) {
            convIm[x + widthConvIm * y] += (-image[(x - 1) + width*y].r + image[(x + 1) + width*y].r) / 255;
        }

        //get the edge cases
        //Using mirror
        convIm[start.x() + widthConvIm * y] += (-image[(end.x() - 1) + width*y].r + image[(start.x() + 1) + width*y].r) / 255;

        convIm[(end.x() - 1) + widthConvIm * y] += (-image[(end.x() - 2) + width*y].r + image[start.x() + width*y].r) / 255;
    }

    //2. convolve with [1 2 1]'
    for (int x = start.x(); x < end.x(); x++) {
        for (int y = start.y() + 1; y < end.y() - 1; y++) {
            convIm[x + widthConvIm * y] += (image[x + width*(y+1)].r +
                                            2*image[x + width*y].r +
                                            image[x + width*(y-1)].r) / 255;
        }

        //get the edge cases
        //Using mirror
        convIm[x + widthConvIm * start.y()] += (image[x + width*(start.y() + 1)].r +
                                                2*image[x + width*start.y()].r +
                                                image[x + width*(end.y() - 1)].r) / 255;

        convIm[x + widthConvIm * (end.y() - 1)] += (image[x + width*start.y()].r +
                                                    2*image[x + width*(end.y() - 1)].r +
                                                    image[x + width*(end.y() - 2)].r) / 255;
    }
}

void EdgeDetectFilter::computeGy(BGRA *image, float *convIm, QPoint &start, QPoint &end, int width)
{
    int widthConvIm = end.x() - start.x();

    //1. first convolve with [1 2 1]
    for (int y = start.y(); y < end.y(); y++) {
        for (int x = start.x() + 1; x < end.x() - 1; x++) {
            convIm[x + widthConvIm * y] += (image[(x - 1) + width*y].r +
                                           2*image[x + width*y].r +
                                           image[(x + 1) + width*y].r) / 255;
        }

        //get the edge cases
        //Using mirror
        convIm[start.x() + widthConvIm * y] += (image[(end.x() - 1) + width*y].r +
                                               2*image[start.x() + width*y].r +
                                               image[(start.x() + 1) + width*y].r) / 255;

        convIm[(end.x() - 1) + widthConvIm * y] += (image[(end.x() - 2) + width*y].r +
                                                   2*image[(end.x() - 1) + width*y].r +
                                                   image[start.x() + width*y].r) / 255;
    }

    //2. convolve with [-1 0 1]'
    for (int x = start.x(); x < end.x(); x++) {
        for (int y = start.y() + 1; y < end.y() - 1; y++) {
            convIm[x + widthConvIm * y] += (-image[x + width*(y+1)].r + image[x + width*(y-1)].r) / 255;
        }

        //get the edge cases
        //Using mirror
        convIm[x + widthConvIm * start.y()] += (-image[x + width*(start.y() + 1)].r + image[x + width*(end.y() - 1)].r) / 255;

        convIm[x + widthConvIm * (end.y() - 1)] += (-image[x + width*start.y()].r + image[x + width*(end.y() - 2)].r) / 255;
    }
}


void EdgeDetectFilter::applyFilter(Canvas2D *canvas, double filterSensitivity)
{
    //First convert the image into greyscale
    GreyscaleFilter::applyFilter(canvas);

    BGRA *image = canvas->data();
    QPoint start = canvas->marqueeStart();
    QPoint end = canvas->marqueeStop();
    int width = canvas->width();
    int height = canvas->height();

    if (!checkBoundary(start, end, width, height))
        return;

    int xSize = end.x() - start.x();
    int ySize = end.y() - start.y();
    float *Gx = new float[xSize * ySize];
    float *Gy = new float[xSize * ySize];
    int a = sizeof(Gx);

    bzero(Gx, sizeof(Gx));
    bzero(Gy, sizeof(Gy));

    computeGx(image, Gx, start, end, width);
    computeGy(image, Gy, start, end, width);

    for (int x = start.x(); x < end.x(); x++) {
        for (int y = start.y(); y < end.y(); y++) {
            image[x + width*y].r = sqrt(Gx[x + width*y]*Gx[x + width*y] + Gy[x + width*y]*Gy[x + width*y]) *
                                   filterSensitivity * 255;
            image[x + width*y].g = sqrt(Gx[x + width*y]*Gx[x + width*y] + Gy[x + width*y]*Gy[x + width*y]) *
                                   filterSensitivity * 255;
            image[x + width*y].b = sqrt(Gx[x + width*y]*Gx[x + width*y] + Gy[x + width*y]*Gy[x + width*y]) *
                                   filterSensitivity * 255;
        }
    }
}


