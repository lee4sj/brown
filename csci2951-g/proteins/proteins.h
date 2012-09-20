#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

/*
 * These were missing - DVV
 */
#define FALSE 0
#define TRUE (!FALSE)

#define DTOR 0.017453292519943297
#define TWOPI           6.283185307179586476925287
#define PI              3.141592653589793238462643
#define PID2            1.570796326794896619231322

#define C_MAX_V .2 
#define C_MAX_V_SQRD (C_MAX_V*C_MAX_V)

typedef struct {
   double x,y,z;
} XYZ;
typedef struct {
   unsigned char r,g,b,a;
} PIXELA;
typedef struct {
   double r,g,b;
} COLOR;

#define ABS(x) (x < 0 ? -(x) : (x))
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define SIGN(x) (x < 0 ? (-1) : 1)
#define MODULUS(p) (sqrt(p.x*p.x + p.y*p.y + p.z*p.z))
#define CROSSPROD(p1,p2,p3) \
   p3.x = p1.y*p2.z - p1.z*p2.y; \
   p3.y = p1.z*p2.x - p1.x*p2.z; \
   p3.z = p1.x*p2.y - p1.y*p2.x

typedef struct {
   XYZ vp;              /* View position           */
   XYZ vd;              /* View direction vector   */
   XYZ vu;              /* View up direction       */
   XYZ pr;              /* Point to rotate about AND point to look at  */
   double focallength;  /* Focal Length along vd   */
   double aperture;     /* Camera aperture         */
   double eyesep,defaulteyesep;       /* Eye separation   IN PIXELS */
   int screenheight,screenwidth;
   double viewheight,defaultviewheight; /*height of window, in position units*/
   double eyedist,defaulteyedist; /*eye distance from the screen, in pixels*/
   double pixelsize;
   double diaglength,defaultdiaglength; //length of screen diagonal
} CAMERA;

typedef struct {
	double *value,loc,start,end,mid,midfrac; //loc is x coordinate of center of knob, relative to start; midfrac is location (from 0 to 1) where scale switches from linear to logarithmic
	int isLogarithmic;//-1 means linear before mid and logarithmic after; otherwise mid is ignored
	int nticks;
	double ticks[10];
} SLIDER;

typedef struct {
	double startx,starty,width,height,knobR; //drawing parameters
	double sliderR,hotspotR; //hotspots of the linear and knob portions respectively
	int jumppixels; //number of pixels to jump when clicked
	double spacingx,spacingy; //spacing of multiple sliders
	int visible,current,nsliders;//current holds index of currently selected slider, for mouse dragging, or potentially keyboard control
	double clickref; //internal, to keep track of last click
	SLIDER sliders[10];
	//other display parameters go here
	double endR;
} SLIDERCONTAINER;

typedef struct {double temp,fric,tempcontrol;} PARAMETERS;

typedef struct {
	int which,dir;//which slider: -1 for none; dir is 0 if we're on the knob, +/-1 to jump left or right
} SLIDERCLICK;

SLIDERCLICK IsOnSlider(double x,double y,SLIDERCONTAINER *sliders);
void HandleSlider(int action,SLIDERCLICK *click,int x,int y,SLIDERCONTAINER *sliders);
void RenderSliders(SLIDERCONTAINER *);
void SliderSet(SLIDER *slider,double *v);
double SliderLoc(SLIDER *slider,double v);
void DrawString(int x,int y,char *s,COLOR col,int which);

void HandleDisplay(void);
void CreateEnvironment(void);
void CreateWorld(void);
//void MakeMesh(void);
void sphereSetup();
void MakeBox(void);
void MakeOctahedron(XYZ p,double r,COLOR c,double ang);
void MakeBands(XYZ p,double r,COLOR c,int n,double w);
void MakeSphere(XYZ c,double r,COLOR color);//n is now a constant
void MakeCylinder(XYZ start,XYZ end,double r,int n,COLOR color);
void MakeCone(XYZ start,XYZ end,double r,int n,COLOR color);
//void MakeSphere(void);
//void MakeKnot(void);
//void MakeTriTorus(void);
//void MakeLorenz(void);
//XYZ  TriTorusEval(double,double);
//void MakePulsar(void);
void MakeLighting(void);
void HandleKeyboard(unsigned char key,int x, int y);
void HandleSpecialKeyboard(int key,int x, int y);
void HandleMouse(int,int,int,int);
void HandleMainMenu(int);
void HandleCameraMenu(int);
void HandleGlassesMenu(int);
void HandleOutputMenu(int);
void HandleModelMenu(int);
void HandleDiagMenu(int);
void HandleSaturationMenu(int whichone);
void HandleAmbientMenu(int whichone);
void HandleDiffuseMenu(int whichone);
void HandleSpecularMenu(int whichone);
void HandleVisibility(int vis);
void HandleReshape(int,int);
void HandleMouseMotion(int,int);
void HandlePassiveMotion(int,int);
void HandleTimer(int);
void GiveUsage(char *);
void RotateCamera(double,double,double);
void TranslateCamera(int,int);
void CameraHome(int);
int  WindowDump(int,int);
void Normalize(XYZ *);
void myexit();
void myexitErr(char *err);
void do3DInput();
void doForce(int*,int,XYZ,XYZ rot,double rotamount);
template <bool> void doPhysics();
XYZ  CalcNormal(XYZ,XYZ,XYZ);
void framerate();
void eigensub(double *change,XYZ *f,XYZ *p,double *out,double delta);
mxArray* eigenSetup(double delta);
void recenter();
int getClosest(XYZ loc);

#define ESC 27

#define MOUSE_NONE 0
#define MOUSE_ROTXY 1
#define MOUSE_ROTZ 2
#define MOUSE_SLIDER 3

#define MOUSE_DOWN 1
#define MOUSE_MOVE 0
#define MOUSE_UP -1
