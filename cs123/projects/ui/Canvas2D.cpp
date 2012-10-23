/**

  CS123 2-dimensional canvas. Contains support code necessary for Brush,
  Filter, Intersect, and Ray.

  YOU WILL NEED TO FILL THIS IN!

  Please contact the authors if there are any bugs.

  @date   2010-08-24
  @author Evan Wallace (edwallac)
  @author Ben Herila (ben@herila.net)

**/

// For your convenience, a few headers are included for you.
#include <math.h>
#include <assert.h>
#include "Canvas2D.h"
#include "Canvas3D.h"
#include "Settings.h"
#include "RayScene.h"


#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"

#include "filter/Filter.h"
#include "filter/InvertFilter.h"
#include "filter/GreyscaleFilter.h"
#include "filter/EdgeDetectFilter.h"
#include "filter/BlurFilter.h"
#include "filter/ScaleFilter.h"
#include "filter/SharpenFilter.h"

Canvas2D::Canvas2D()
{
    // @TODO: Initialize any pointers in this class here.
    m_scene = NULL;
    brush = NULL;

    filters[FILTER_INVERT] = new InvertFilter();
    filters[FILTER_GREYSCALE] = new GreyscaleFilter();
    filters[FILTER_EDGE_DETECT] = new EdgeDetectFilter();
    filters[FILTER_BLUR] = new BlurFilter();
    filters[FILTER_SCALE] = new ScaleFilter();
    filters[FILTER_SPECIAL_1] = new SharpenFilter();
}

Canvas2D::~Canvas2D()
{
    // @TODO: Be sure to release all memory that you allocate.
    delete m_scene;

    //Brush
    if (brush != NULL)
        delete brush;
}

// This is called when the canvas size is changed. You can change the canvas size
// by calling resize(...). You probably won't need to fill this in, but you can
// if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {

}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override
    // any painting events for the 2D canvas. For now, we simply call the superclass.

    SupportCanvas2D::paintEvent(e);

}

// *****************************************************************************************
// ** BRUSH
// *****************************************************************************************

void Canvas2D::mouseDown(int x, int y)
{
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a
    //        separate class for each of your brushes. Remember that you can use the
    //        static Settings object to get the currently selected brush and its parameters.

    int currentBrush = settings.brushType;
    int currentRadius = settings.brushRadius;

    BGRA currentColor;
    currentColor.b = settings.brushBlue;
    currentColor.g = settings.brushGreen;
    currentColor.r = settings.brushRed;
    currentColor.a = 255;

    int currentFlow = settings.brushAlpha;

    if (brush != NULL) {
        delete brush;
        brush = NULL;
    }

    switch (currentBrush) {
    case BRUSH_SOLID:
        brush = new ConstantBrush(currentColor, currentFlow, currentRadius);
        break;
    case BRUSH_LINEAR:
        brush = new LinearBrush(currentColor, currentFlow, currentRadius);
        break;
    case BRUSH_QUADRATIC:
        brush = new QuadraticBrush(currentColor, currentFlow, currentRadius);
        break;
    case BRUSH_SMUDGE:
        brush = new SmudgeBrush(currentColor, currentFlow, currentRadius);
        ((SmudgeBrush *)brush)->pickUpPaint(x, y, this);
        break;
    default:
        if (brush != NULL)
            delete brush;
        brush = NULL;
        break;
    }
    // you're going to need to leave the alpha value on the canvas itself at 255, but
    // you will need to use the actual alpha value to compute the new color of the pixel

}

void Canvas2D::mouseDragged(int x, int y)
{
    // TODO: [BRUSH] Mouse interaction for Brush.
    if (!brush)
        return;

    brush->paintOnce(x, y, this);
}

void Canvas2D::mouseUp(int x, int y)
{
    // TODO: [BRUSH] Mouse interaction for Brush.
    if (!brush)
        return;

    brush->paintOnce(x, y, this);
    delete brush;
    brush = NULL;
}



// *****************************************************************************************
// ** FILTER
// *****************************************************************************************

void Canvas2D::filterImage()
{
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.

    switch (settings.filterType) {
    case FILTER_EDGE_DETECT:
        ((EdgeDetectFilter *)filters[FILTER_EDGE_DETECT])->applyFilter(this, settings.edgeDetectThreshold);
        break;
    case FILTER_BLUR:
        ((BlurFilter *)filters[FILTER_BLUR])->applyFilter(this, settings.blurRadius);
        break;
    case FILTER_SCALE:
        ((ScaleFilter *)filters[FILTER_SCALE])->applyFilter(this, settings.scaleX, settings.scaleY);
        break;
    default:
        if (filters[settings.filterType])
            filters[settings.filterType]->applyFilter(this);
        break;
    }
    this->update();
}

void Canvas2D::setScene(RayScene *scene)
{
    delete m_scene;
    m_scene = scene;
}


void Canvas2D::renderImage(Camera *camera, int width, int height)
{
    if (m_scene)
    {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering

    }
}

void Canvas2D::cancelRender()
{
    // TODO: cancel the raytracer (optional)
}



void Canvas2D::settingsChanged() {

    // TODO: Process changes to the application settings.

}
