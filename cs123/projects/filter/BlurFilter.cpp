#include "BlurFilter.h"

BlurFilter::BlurFilter() {}

BlurFilter::~BlurFilter() {}

void BlurFilter::getGaussian(double *kernel, int r)
{
    double sigma = r / 3;
    double c = 1/sqrt(2 * M_PI * sigma);
    kernel[r] = c;
    for (int i = 1; i <= r; i++) {
        kernel[r+i] = c / exp((i*i) / (2*sigma*sigma));
        kernel[r-i] = kernel[r+1];
    }
}

void BlurFilter::applyHorizontalKernel(BGRA *image, BGRA *convIm, double *kernel,
                                       QPoint &start, QPoint &end, int width, int r)
{
    int convWidth = end.x() - start.x();

    for (int y = start.y(); y < end.y(); y++) {
        for (int x = start.x() + r; x < end.x() - r; x++) {
            //this is convolving the inner parts
            for (int i = -r; i <= r; i++) {
                convIm[x + convWidth*y].r += kernel[(x + i) + convWidth*y] * image[(x + i) + width*y].r;
                convIm[x + convWidth*y].g += kernel[(x + i) + convWidth*y] * image[(x + i) + width*y].g;
                convIm[x + convWidth*y].b += kernel[(x + i) + convWidth*y] * image[(x + i) + width*y].b;
            }
        }

        //to convolve the edges using mirror
        for (int x = start.x(); x < start.x() + r; x++) {
            int numOutOfBoundary = r - (x - start.x());
            for (int i = -r + numOutOfBoundary; i <= r; i++) {
                convIm[x + convWidth*y].r += kernel[(x + i) + convWidth*y] * image[(x + i) + width*y].r;
                convIm[x + convWidth*y].g += kernel[(x + i) + convWidth*y] * image[(x + i) + width*y].g;
                convIm[x + convWidth*y].b += kernel[(x + i) + convWidth*y] * image[(x + i) + width*y].b;
            }
            for (int i = -(x - start.x()); i < numOutOfBoundary - (x - start.x()); i++) {
                convIm[x + convWidth*y].r += kernel[(x + i) + convWidth*y] * image[(x + i) + width*y].r;
                convIm[x + convWidth*y].g += kernel[(x + i) + convWidth*y] * image[(x + i) + width*y].g;
                convIm[x + convWidth*y].b += kernel[(x + i) + convWidth*y] * image[(x + i) + width*y].b;
            }
        }
    }
}

void BlurFilter::applyFilter(Canvas2D *canvas, int radius)
{
    BGRA *image = canvas->data();
    QPoint start = canvas->marqueeStart();
    QPoint end = canvas->marqueeStop();
    int width = canvas->width();
    int height = canvas->height();

    if (!checkBoundary(start, end, width, height))
        return;

    if ((2*radius + 1) > width)
        radius = (width - 1)/2;

    double kernel[(2*radius + 1)];
    getGaussian(kernel, radius);

    int xSize = end.x() - start.x();
    int ySize = end.y() - start.y();

    BGRA convIm[xSize * ySize];

    applyHorizontalKernel(image, convIm, kernel, start, end, width, radius);



}
