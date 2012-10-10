#include "EdgeDetectFilter.h"
#include "filter/GreyscaleFilter.h"

EdgeDetectFilter::EdgeDetectFilter() {}

EdgeDetectFilter::~EdgeDetectFilter() {}

void EdgeDetectFilter::computeGx(BGRA *image, float convIm[], QPoint &start, QPoint &end, int width)
{
    int widthConvIm = end.x() - start.x();
    int heightConvIm = end.y() - start.y();

    float temp[heightConvIm * widthConvIm];

    //1. first convolve with [-1 0 1]
    for (int y = start.y(); y < end.y(); y++) {
        for (int x = start.x() + 1; x < end.x() - 1; x++) {
            temp[x + widthConvIm * y] = (-image[(x - 1) + width*y].r + image[(x + 1) + width*y].r) / 255.0;
        }

        //get the edge cases
        //Using mirror
        temp[start.x() + widthConvIm * y] = 0;

        temp[(end.x() - 1) + widthConvIm * y] = 0;
    }

    //2. convolve with [1 2 1]'
    for (int x = start.x(); x < end.x(); x++) {
        for (int y = start.y() + 1; y < end.y() - 1; y++) {
            convIm[x + widthConvIm * y] = temp[x + widthConvIm*(y+1)] +
                                          2*temp[x + widthConvIm*y] +
                                          temp[x + widthConvIm*(y-1)];
        }

        //get the edge cases
        //Using mirror
        convIm[x + widthConvIm * start.y()] = 2*temp[x + widthConvIm*(start.y() + 1)] +
                                              2*temp[x + widthConvIm*start.y()];

        convIm[x + widthConvIm * (end.y() - 1)] = 2*temp[x + widthConvIm*(end.y() - 1)] +
                                                  2*temp[x + widthConvIm*(end.y() - 2)];
    }
}

void EdgeDetectFilter::computeGy(BGRA *image, float *convIm, QPoint &start, QPoint &end, int width)
{
    int widthConvIm = end.x() - start.x();
    int heightConvIm = end.y() - start.y();

    float temp[heightConvIm * widthConvIm];

    //1. first convolve with [1 2 1]
    for (int y = start.y(); y < end.y(); y++) {
        for (int x = start.x() + 1; x < end.x() - 1; x++) {
            temp[x + widthConvIm * y] = (image[(x - 1) + width*y].r +
                                         2*image[x + width*y].r +
                                         image[(x + 1) + width*y].r) / 255.0;
        }

        //get the edge cases
        //Using mirror
        temp[start.x() + widthConvIm * y] = (2*image[start.x() + width*y].r +
                                             2*image[(start.x() + 1) + width*y].r) / 255.0;

        temp[(end.x() - 1) + widthConvIm * y] = (2*image[(end.x() - 2) + width*y].r +
                                                 2*image[(end.x() - 1) + width*y].r) / 255.0;
    }

    //2. convolve with [-1 0 1]'
    for (int x = start.x(); x < end.x(); x++) {
        for (int y = start.y() + 1; y < end.y() - 1; y++) {
            convIm[x + widthConvIm * y] = -temp[x + widthConvIm*(y+1)] + temp[x + widthConvIm*(y-1)];
        }

        //get the edge cases
        //Using mirror
        convIm[x + widthConvIm * start.y()] = 0;

        convIm[x + widthConvIm * (end.y() - 1)] = 0;
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

    if (filterSensitivity < 0 || filterSensitivity > 1)
        filterSensitivity = 0.5;

    int xSize = end.x() - start.x();
    int ySize = end.y() - start.y();
    float Gx[xSize * ySize];
    float Gy[xSize * ySize];

    computeGx(image, Gx, start, end, width);
    computeGy(image, Gy, start, end, width);

    float val = 0;
    int indexIm = 0;
    int indexConvIm = 0;
    for (int x = start.x(); x < end.x(); x++) {
        for (int y = start.y(); y < end.y(); y++) {
            indexConvIm = x + xSize*y;
            indexIm = x + width*y;
            val = sqrt(Gx[indexConvIm]*Gx[indexConvIm] + Gy[indexConvIm]*Gy[indexConvIm]) * filterSensitivity ;
            val = fmax(0, fmin(val, 1)) * 255;
            image[indexIm].r = val;
            image[indexIm].g = val;
            image[indexIm].b = val;
        }
    }
}


