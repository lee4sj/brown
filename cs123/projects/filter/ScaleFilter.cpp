#include "ScaleFilter.h"

ScaleFilter::ScaleFilter() {}

ScaleFilter::~ScaleFilter() {}


//void ScaleFilter::scaleDown(Canvas2D *canvas, BGRA *im, int width, int height,
//                            int newWidth, int newHeight)
//{
//    BlurFilter::applyFilter(canvas, 3);

//    BGRA *canvasIm = canvas->data();

//    double xStep = width / newWidth;
//    double yStep = height / newHeight;
//    for (int yCanv = 0; yCanv < newHeight; yCanv++) {
//        for (int xCanv = 0; xCanv < newWidth; xCanv++) {
//            canvasIm[xCanv + newWidth*yCanv] = im[xStep*xCanv + width * yStep * yCanv];
//        }
//    }
//}


//void horizontalComputePixel(BGRA *int xIm, int radius)
//{
//    int radius = 1/scaleX;
//    float r=0, g=0, b=0, total=0;

//    for (int i = -radius; i <= radius; i++)
//    {
//        int x = min(max(xIm + i, 0), width - 1);
//        float dist = x - xIm;

//        r += ((1 - fabs(dist)/radius) / radius) * im[xIm + width*yIm].r;
//        g += ((1 - fabs(dist)/radius) / radius) * im[xIm + width*yIm].g;
//        b += ((1 - fabs(dist)/radius) / radius) * im[xIm + width*yIm].b;
//        total += ((1 - fabs(dis)/radius) / radius);
//    }
//}


void ScaleFilter::applyFilter(Canvas2D *canvas, double scaleX, double scaleY)
{
    BGRA *canvasIm = canvas->data();
    int width = canvas->width();
    int height = canvas->height();
    QPoint start = QPoint(0, 0);
    QPoint end = QPoint(width, height);

    int newWidth = width * scaleX;
    int newHeight = height * scaleY;

    BGRA *im = new BGRA[width * height];
    memcpy(im, canvasIm, width * height * sizeof(BGRA));


    //scale horizontally
    //first blur using gaussian horizontally.
    if (scaleX < 1) {
        int radius = ceil(1.0/scaleX);
        double *kernel = new double[(2*radius + 1)];
        getGaussian(kernel, radius);

        BGRADouble *tempIm = new BGRADouble[width * height];
        applyHorizontalKernel(tempIm, im, width, kernel, start, end, radius);

        //now sample
        canvas->resize(newWidth, height);
        canvasIm = canvas->data();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < newWidth; x++) {
                canvasIm[x + newWidth * y].r = tempIm[(int)(x/scaleX + width * y)].r * 255;
                canvasIm[x + newWidth * y].g = tempIm[(int)(x/scaleX + width * y)].g * 255;
                canvasIm[x + newWidth * y].b = tempIm[(int)(x/scaleX + width * y)].b * 255;
            }
        }

        delete[] tempIm;
        delete[] kernel;
    }

    else if (scaleX > 1) {
        int xIm = 0;
        int r, g, b;
        double weight, total;

        canvas->resize(newWidth, height);
        canvasIm = canvas->data();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < newWidth; x++) {
                r = 0, g = 0, b = 0;
                total = 0;
                for (int i = -1; i <= 1; i++) {
                    xIm = min(max((int)floor(x/scaleX) + i, 0), width - 1);

                    if (fabs(xIm - x/scaleX) > 1)
                        continue;

                    weight = (1 - fabs(xIm - x/scaleX));

                    r += weight * im[xIm + width*y].r;
                    g += weight * im[xIm + width*y].g;
                    b += weight * im[xIm + width*y].b;
                    total += weight;
                }

                canvasIm[x + newWidth * y].r = r/total;
                canvasIm[x + newWidth * y].g = g/total;
                canvasIm[x + newWidth * y].b = b/total;
            }
        }
    }


    delete[] im;

    width = newWidth;
    im = new BGRA[width * height];
    memcpy(im, canvasIm, width * height * sizeof(BGRA));
    end = QPoint(width, height);

    //scale Vertically
    if (scaleY < 1) {
        int radius = ceil(1.0/scaleY);
        double *kernel = new double[(2*radius + 1)];
        getGaussian(kernel, radius);

        BGRADouble *tempIm = new BGRADouble[width * height];
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                tempIm[x + width*y].r = im[x + width*y].r / 255.0;
                tempIm[x + width*y].g = im[x + width*y].g / 255.0;
                tempIm[x + width*y].b = im[x + width*y].b / 255.0;
            }
        }
        applyVerticalKernel(im, tempIm, width, kernel, start, end, radius);

        //now sample
        canvas->resize(width, newHeight);
        canvasIm = canvas->data();

        for (int y = 0; y < newHeight; y++) {
            for (int x = 0; x < width; x++) {
                canvasIm[x + width * y].r = im[(int)(x + width * y/scaleY)].r;
                canvasIm[x + width * y].g = im[(int)(x + width * y/scaleY)].g;
                canvasIm[x + width * y].b = im[(int)(x + width * y/scaleY)].b;
            }
        }

        delete[] tempIm;
        delete[] kernel;
    }

    else if (scaleY > 1) {
        int yIm = 0;
        int r, g, b;
        double weight, total;

        canvas->resize(width, newHeight);
        canvasIm = canvas->data();

        for (int y = 0; y < newHeight; y++) {
            for (int x = 0; x < width; x++) {
                r = 0, g = 0, b = 0;
                total = 0;
                for (int i = -1; i <= 1; i++) {
                    yIm = min(max((int)floor(y/scaleY) + i, 0), height - 1);

                    if (fabs(yIm - y/scaleY) > 1)
                        continue;

                    weight = (1 - fabs(yIm - y/scaleY));

                    r += weight * im[x + width*yIm].r;
                    g += weight * im[x + width*yIm].g;
                    b += weight * im[x + width*yIm].b;
                    total += weight;
                }

                canvasIm[x + width * y].r = r/total;
                canvasIm[x + width * y].g = g/total;
                canvasIm[x + width * y].b = b/total;
            }
        }
    }

    delete[] im;
}


















