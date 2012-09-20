#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef __unix__
#include <pthread.h>
#include <GL/glx.h>
//#define LINUX_INPUT
#ifdef LINUX_INPUT
#include <spnav.h>
#endif
#endif
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include "mex.h"
#include "proteins.h"

//LOAD DATA:
//d=loaddat;d2=loadproteinalpha(d,{'nasn','leu','tyr','ile','gln','trp','leu','lys','asp','gly','gly','pro','ser','ser','gly','arg','pro','pro','pro','cser'});p=loadphys;p2=applyphys(d2,p);

//COMPILE:
//ON WINDOWS:   clear mex; mex -I. -L. -lfreeglut proteins.cpp
//ON LINUX with 3D mouse installed: mex -DLINUX_INPUT proteins.cpp -lglut -lGL -lGLU -lpthread -Ispnav -Lspnav -lspnav -lX11
//ON LINUX without 3D mouse support: mex proteins.cpp -lglut -lGL -lGLU -lpthread

//RUN:
//proteins(d2.p,d2.edges,d2.types,p2);

/*
   Paul Valiant (pvaliant@gmail.com)
   Code released for Brown University's CSCI 2951-G: Computational Protein Folding

   Rendering based on code from Paul Bourke, modified by Daniel van Vugt
*/

/* Misc globals */

mxArray *M_outp; int nouts;
int implicitwater,nvfix,writefile=0,writefreq=100,simiters,justrecentered;
FILE *filepointer;
double (*v_table)[64][3],(*e_table)[64][3],(*ef_table)[64][3];
int runningonmylaptop,numberofevaluations,numberofVDWs;
int running,count,timebase,frame,doframerate=1,pause,displayforces,atomdisplay,singleatomforces;
int showcursor,nglued,*glued,npush,*push,outputcount,exitAfterPhysics,doenergies;
bool *drawbig,*drawbig_inp,allbig,allsmall; double smallscale,bigscale,bondmomentum,*FORCE_M;
double *out_P,*out_V,*out_F;
double ambientstrength,diffusestrength,specularstrength,saturation,pushscale;
int nstringstodraw;
char s[50],stringstodraw[10][50];
SLIDERCONTAINER allsliders;
PARAMETERS parms;
int mousedragmeaning,noutput;
double dtheta = 1.0;         /* Camera rotation angle increment */
int G_nlhs;
mxArray** G_plhs;
CAMERA camera;
XYZ origin = {0.0,0.0,0.0},cursorpos;
static int xlast=-1,ylast=-1;
int physicsOn;
//int fricchange; double fric1,fric2;
float (*sphereP)[3];
int sphereinit,spherelist;

/* Image saving options */
int windowdump = FALSE;

/* Glasses filter types */
#define REDBLUE  1
#define REDGREEN 2
#define REDCYAN  3
#define BLUERED  4
#define GREENRED 5
#define CYANRED  6
int glassestype = REDCYAN;
XYZ *verts,*angloc,*angvel,*angfor,*angvelmax,*angmomentum;
int nvert,nedge,nVDW,(*edges)[2],(*bonds)[2],(*angles)[2],nangles,have3DInput,activexinput,*parent;
int (*dihedrals)[6],(*improper)[2],ndihedrals,nimproper;
double (*bondparms)[2],(*weighti),(*angparams)[2],simspeed,fric=0.999;
double (*dihparams)[3],(*impparams)[3];
char (*types)[2];
XYZ *p_F,*p_F2,*p_V,*p_force,*p_Fimplicit1,*p_Fimplicit2,*p_Fvdw,*p_Fsingle;
mxArray *srv;
XYZ *VDW; int *VDW1,*VDW2;
int waterlastupdated;
double *charges,(*vdw)[2];//vdw: radius, depth
int *atomtype;
int (*special_raw)[3],nspecial_raw;//edges for special processing: vert1,vert2,type (0 for ignore, 1 for scale down)
int (*special)[2];
double *special_dat1,(*special_dat3)[3];
int *G_inds,*G_gluet;
XYZ *G_iv,*G_ds,*G_fs,*G_angc;
double *G_lengthi,*G_sint,*G_cost,*scratch,*scratchi,GGB,*Energies;


void output(double *data,int length) {
    mxArray *dat;
	if (nouts<100) {
    dat=mxCreateNumericMatrix(1,length,mxDOUBLE_CLASS,mxREAL);
    memcpy(mxGetPr(dat),data,length*sizeof(double));
    mxSetCell(M_outp,nouts,dat);//not very elegant way of commenting this out; also, this output will clash with eig/V output???
	nouts++;}
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	/* Set things (glut) up */
	int i,j,k,*pi,i1,i2,i3,i4,o1,o2,o3,o4,e1,e2;
	double *pp;
	mxArray *pm,*p2[2]; 
	mxChar *pc;
	int mainmenu,modelmenu,outputmenu,cameramenu,glassesmenu,satmenu,ambmenu,difmenu,specmenu,diagmenu;
	char *argv[]={"foo"},str[50]; int argc=1;
	const mwSize *size;
	SLIDER *sliders=allsliders.sliders;
	XYZ zero={0,0,0};
//	e1=UnregisterClass("FREEGLUT",0); //@@
	running=0;
	//	e2=GetLastError();
	//	if (e2) mexPrintf("%d,%d\n",e1,e2);
	nouts=0;M_outp=mxCreateCellMatrix(1,1000); plhs[0]=M_outp;nvfix=0; charges=0; vdw=0; 
	justrecentered=0;special_raw=0;special_dat1=0;special_dat3=0;nspecial_raw=0;
	sphereP=(float (*) [3])malloc(10000*3*sizeof(float)); //spherelastn=0;
	nstringstodraw=0; outputcount=-1; parms.fric=.5; parms.temp=300; parms.tempcontrol=.01; pushscale=1; simiters=5;
	allsliders.startx=-205;allsliders.starty=-30;allsliders.width=150;allsliders.height=6;allsliders.knobR=8;
	allsliders.endR=3;
	allsliders.sliderR=4;allsliders.hotspotR=10;allsliders.jumppixels=20;allsliders.spacingx=0;allsliders.spacingy=-30;
	allsliders.visible=1;allsliders.nsliders=3;
	sliders[0].start=0;sliders[0].end=1;sliders[0].mid=.0001;sliders[0].midfrac=.2;sliders[0].isLogarithmic=-1;sliders[0].nticks=6;
	pp=sliders[0].ticks;pp[0]=0;pp[1]=.0001;pp[2]=.001;pp[3]=.01;pp[4]=.1;pp[5]=1;
	pp=sliders[1].ticks;pp[0]=0;pp[1]=10;pp[2]=30;pp[3]=100;pp[4]=300;pp[5]=999;
	pp=sliders[2].ticks;pp[0]=0;pp[1]=.0001;pp[2]=.001;pp[3]=.01;pp[4]=.1;pp[5]=1;
	sliders[1].start=0;sliders[1].end=999;sliders[1].mid=10;sliders[1].midfrac=.2;sliders[1].isLogarithmic=-1;sliders[1].nticks=6;
	sliders[2].start=0;sliders[2].end=1;sliders[2].mid=.0001;sliders[2].midfrac=.2;sliders[2].isLogarithmic=-1;sliders[2].nticks=6;
	SliderSet(&sliders[0],&parms.fric);SliderSet(&sliders[1],&parms.temp);SliderSet(&sliders[2],&parms.tempcontrol);
	mousedragmeaning=MOUSE_NONE;displayforces=0;atomdisplay=0;
	ambientstrength=.1,diffusestrength=.4,specularstrength=.4;saturation=1;
//	fricchange=1000;fric1=.5;fric2=.999;
	simspeed=.0004; sphereinit=0; allbig=0;allsmall=0;singleatomforces=-1;
	smallscale=.7; bigscale=2; //smallscale=bigscale=1;//###
	noutput=1000; exitAfterPhysics=0;
	G_plhs=plhs; G_nlhs=nlhs;
	p_F=p_V=p_force=VDW=NULL; count=0; timebase=0; frame=0; memset(s,0,50); pause=1;
	if (nrhs==3) {mexErrMsgTxt("Cannot have 3 arguments!");physicsOn=0;} else if (nrhs==4) physicsOn=1; else mexErrMsgTxt("Must have 3 or 4 input arguments!");
	size=mxGetDimensions(prhs[0]);
	if (size[1]!=3) mexErrMsgTxt("First input, vertex locations, must have 3 columns!");
	nvert=size[0]; if (nvert>15000) mexErrMsgTxt("More than 15000 vertices is a bad idea!");
	glued=(int*)malloc(nvert*sizeof(int));
	push=(int*)malloc(nvert*sizeof(int));
	nglued=0; glued[0]=-1;showcursor=0; npush=1; push[0]=0;
	verts=(XYZ*)malloc((nvert+2)*sizeof(XYZ)); verts[nvert].x=0;verts[nvert].y=-1;verts[nvert].z=0;verts[nvert+1].x=-1;verts[nvert+1].y=-1;verts[nvert+1].z=0;
	weighti=(double*)malloc(nvert*sizeof(double));
	types=(char(*)[2])malloc(nvert*sizeof(char[2]));
	G_inds=(int*)malloc(nvert*sizeof(int));
	G_iv=(XYZ*)malloc(nvert*sizeof(XYZ));
	G_gluet=(int*)malloc(nvert*sizeof(int));
	p_F=(XYZ*)malloc(nvert*sizeof(XYZ));p_V=(XYZ*)malloc((nvert+2)*sizeof(XYZ));memset(p_V+nvert,0,sizeof(XYZ)*2);
	p_force=(XYZ*)malloc(nvert*sizeof(XYZ));
	p_F2=(XYZ*)malloc(nvert*sizeof(XYZ));
	p_Fimplicit1=(XYZ*)malloc(nvert*sizeof(XYZ));p_Fimplicit2=(XYZ*)malloc(nvert*sizeof(XYZ));
	p_Fvdw=(XYZ*)malloc(nvert*sizeof(XYZ));p_Fsingle=(XYZ*)malloc(nvert*sizeof(XYZ));
	drawbig=(bool*)malloc(nvert*sizeof(bool));
	drawbig_inp=(bool*)malloc(nvert*sizeof(bool));
	scratch=(double*)malloc((nvert+2)*21*sizeof(double));
	size=mxGetDimensions(prhs[1]);
	if (size[1]!=2) mexErrMsgTxt("Second input, edge pairs, must have 2 columns!");
	nedge=size[0]; if (nedge>100000) mexErrMsgTxt("More than 100000 edges is a bad idea!");
	edges=(int(*)[2])malloc(nedge*sizeof(int[2]));
	bonds=(int(*)[2])malloc(nedge*sizeof(int[2]));
	bondparms=(double(*)[2])malloc(nedge*sizeof(double[2]));
	G_ds=(XYZ*)malloc(nedge*sizeof(XYZ));
	G_fs=(XYZ*)malloc(nedge*sizeof(XYZ));
	G_lengthi=(double*)malloc(nedge*sizeof(double));
	if (sizeof(XYZ)!=3*sizeof(double)) mexErrMsgTxt("XYZ size isn't 3 times double size!");

	pp=(double*)mxGetData(prhs[0]);
	for (i=0;i<nvert;i++) {verts[i].x=pp[i]; verts[i].y=pp[i+nvert]; verts[i].z=pp[i+2*nvert];}
	pi=(int*)mxGetData(prhs[1]); if (mxGetClassID(prhs[1])!=mxINT32_CLASS) mexErrMsgTxt("Second input must be int32!");
	for (i=0;i<nedge;i++) {edges[i][0]=pi[i]-1;edges[i][1]=pi[i+nedge]-1;};
	if (nvert!=mxGetNumberOfElements(prhs[2])) mexErrMsgTxt("Third argument must have one entry per vertex!");
	for (i=0;i<nvert;i++) {
		if (!(pm=mxGetCell(prhs[2],i))) mexErrMsgTxt("Cannot read types!"); 
		if (mxGetClassID(pm)!=mxCHAR_CLASS) mexErrMsgTxt("Types contains non-char data!"); pc=(mxChar*)mxGetData(pm);
		k=mxGetNumberOfElements(pm); if ((k<1) || (k>2)) mexErrMsgTxt("Types must have length 1 or 2!");
		types[i][1]=0; for (j=0;j<k;j++) types[i][j]=pc[j];
	}
	if (physicsOn) {
		pm=mxGetField(prhs[3],0,"big");
		if (!pm) {allbig=1; for (i=0;i<nvert;i++) drawbig[i]=(types[i][1]!='W')||(types[i][0]=='C');}
		else {
			for (i=0;i<nvert;i++) drawbig[i]=false;
			if (mxGetClassID(pm)!=mxDOUBLE_CLASS) mexErrMsgTxt("`Big' must be an array of vertices!");
			k=mxGetNumberOfElements(pm); pp=(double*)mxGetData(pm);
			for (i=0;i<k;i++) {j=(int)pp[i]-1; if ((j>=0) && (j<nvert)) drawbig[j]=true; else mexErrMsgTxt("Out of range `big' index!");}
		}
		memcpy(drawbig_inp,drawbig,nvert*sizeof(bool));

		pm=mxGetField(prhs[3],0,"bonds"); if (!pm) mexErrMsgTxt("4th argument must describe bonds!");
		size=mxGetDimensions(pm); if ((size[0]!=nedge) || (size[1]!=4)) mexErrMsgTxt("Bonds must be an nedge by 4 matrix");
		pp=(double*)mxGetData(pm); 
		for (i=0;i<nedge;i++) {bonds[i][0]=(int)pp[i]-1; bonds[i][1]=(int)pp[i+nedge]-1; bondparms[i][0]=pp[i+2*nedge]*2; bondparms[i][1]=pp[i+3*nedge];}
		parent=(int*)malloc((nvert+2)*sizeof(int));for(i=0;i<nvert;i++) {parent[i]=nvert;for(j=0;j<nedge;j++) {int p=nvert;if (bonds[j][0]==i) p=bonds[j][1]; if (bonds[j][1]==i) p=bonds[j][0]; if (p<parent[i]) parent[i]=p;}}parent[0]=nvert;parent[nvert]=nvert+1;
//for (i=0;i<nvert;i++) printf("%d: %d\n",i,parent[i]);
		pm=mxGetField(prhs[3],0,"bondmomentum"); if (pm) bondmomentum=*(double*)mxGetData(pm); else bondmomentum=0;
		pm=mxGetField(prhs[3],0,"angmomentum"); if (pm) {
			if (mxGetM(pm)!=nvert || mxGetN(pm)!=3) mexErrMsgTxt("angmomentum wrong size!");
			angmomentum=(XYZ*)malloc(nvert*sizeof(XYZ)); pp=(double*)mxGetData(pm); for (int i=0;i<nvert;i++) {angmomentum[i].x=pp[i];angmomentum[i].y=pp[i+nvert];angmomentum[i].z=pp[i+2*nvert];}
			angvelmax=(XYZ*)malloc(nvert*sizeof(XYZ));angloc=(XYZ*)malloc((nvert+2)*sizeof(XYZ));angvel=(XYZ*)malloc((nvert+2)*sizeof(XYZ));angfor=(XYZ*)malloc(nvert*sizeof(XYZ));
			if (pm=mxGetField(prhs[3],0,"angvelmax")) {
				if (mxGetM(pm)!=nvert || mxGetN(pm)!=3) mexErrMsgTxt("angvelmax wrong size!");
				pp=(double*)mxGetData(pm); for (int i=0;i<nvert;i++) {angvelmax[i].x=pp[i];angvelmax[i].y=pp[i+nvert];angvelmax[i].z=pp[i+2*nvert];}
			} else for (i=0;i<nvert;i++) angvelmax[i].x=angvelmax[i].y=angvelmax[i].z=.1;
		} else angmomentum=0;
		pm=mxGetField(prhs[3],0,"weights"); if (!pm) mexErrMsgTxt("4th argument must describe weights!");
		if (nvert!=mxGetNumberOfElements(pm)) mexErrMsgTxt("Must have one weight per vertex!");
		pp=(double*)mxGetData(pm);
		pm=mxGetField(prhs[3],0,"simstep"); if (pm) simspeed=*(double*)mxGetData(pm);
		for (i=0;i<nvert;i++) weighti[i]=simspeed/pp[i];
		memset(p_V,0,nvert*sizeof(XYZ));
		if (pm=mxGetField(prhs[3],0,"M")) {if ((mxGetM(pm)!=nvert) || (mxGetN(pm)!=nvert)) mexErrMsgTxt("M must be nvert by nvert"); else FORCE_M=(double*)mxGetData(pm);} else FORCE_M=0;
		if (pm=mxGetField(prhs[3],0,"pause")) pause=(int)*(double*)mxGetData(pm);
		if (pm=mxGetField(prhs[3],0,"drawevery")) simiters=(int)*(double*)mxGetData(pm); if (simiters<=0) mexErrMsgTxt("drawevery must be positive");
		if (pm=mxGetField(prhs[3],0,"pushscale")) pushscale=*(double*)mxGetData(pm);
		if (pm=mxGetField(prhs[3],0,"screensize")) camera.defaultdiaglength=*(double*)mxGetData(pm)*25.4; else camera.defaultdiaglength=0;
		pm=mxGetField(prhs[3],0,"params"); //optional field to specify 3 sliders in bottom left
		if (pm) {
			if (mxGetNumberOfElements(pm)!=3) mexErrMsgTxt("Params must have 3 elements!");
			pp=(double*)mxGetData(pm); parms.tempcontrol=pp[0]; parms.temp=pp[1]; parms.fric=pp[2]; //fricchange=(int)pp[0]; fric1=pp[1]; fric2=pp[2];
		}
		if (pm=mxGetField(prhs[3],0,"output")) {
			noutput=(int)*(double*)mxGetData(pm);
			if (noutput>100000) mexErrMsgTxt("Number of outputs must be less than 100000");
			if (noutput>0) HandleOutputMenu(2); //output noutput P,V,F's, and then exit
			if (noutput==-1) exitAfterPhysics=1;
		}
		pm=mxGetField(prhs[3],0,"glue"); //optional field to specify glued vertices
		if (pm) {
			nglued=mxGetNumberOfElements(pm); if (nglued>nvert) mexErrMsgTxt("Too many glued vertices!");
			pp=(double*)mxGetData(pm); for (i=0;i<nglued;i++) {glued[i]=(int)pp[i]-1; if ((glued[i]<0) || (glued[i]>=nvert)) mexErrMsgTxt("Invalid glued vertex!");}
		}
		pm=mxGetField(prhs[3],0,"velocity"); //optional field to specify initial velocities
		if (pm) {
			size=mxGetDimensions(pm); if (size[1]!=3) mexErrMsgTxt("Velocity must have 3 columns!");
			if (size[0]!=nvert) mexErrMsgTxt("Velocity must have an entry per vertex!");
			pp=(double*)mxGetData(pm);for (i=0;i<nvert;i++) {p_V[i].x=pp[i];p_V[i].y=pp[i+nvert];p_V[i].z=pp[i+2*nvert];}
		}
		pm=mxGetField(prhs[3],0,"angles"); 
		if (!pm) nangles=0; else {//mexErrMsgTxt("4th argument must describe angles!");
			size=mxGetDimensions(pm); if (size[1]!=4) mexErrMsgTxt("Angles must have 4 columns!");
			nangles=size[0]; pp=(double*)mxGetData(pm);
			G_angc=(XYZ*)malloc(nangles*sizeof(XYZ));
			G_sint=(double*)malloc(nangles*sizeof(double));
			G_cost=(double*)malloc(nangles*sizeof(double));
			angles=(int(*)[2])malloc(nangles*sizeof(int[2]));
			angparams=(double(*)[2])malloc(nangles*sizeof(double[2]));
			for (i=0;i<nangles;i++) {angles[i][0]=(int)pp[i]-1;angles[i][1]=(int)pp[i+nangles]-1;angparams[i][0]=pp[i+2*nangles]*2;angparams[i][1]=pp[i+3*nangles]*DTOR;}
		}
		//force in radians, angle in degrees
		pm=mxGetField(prhs[3],0,"dihedrals"); 
		if (!pm) ndihedrals=0; else {//mexErrMsgTxt("4th argument must describe dihedrals!");
			size=mxGetDimensions(pm); if (size[1]!=6) mexErrMsgTxt("Dihedrals must have 6 columns!");
			ndihedrals=size[0]; pp=(double*)mxGetData(pm); if (ndihedrals>100000) mexErrMsgTxt("More than 100000 dihedrals is a bad idea!");
			dihedrals=(int(*)[6])malloc(ndihedrals*sizeof(int[6]));
			dihparams=(double(*)[3])malloc(ndihedrals*sizeof(double[3]));
			for (i=0;i<ndihedrals;i++) {
				j=dihedrals[i][0]=(int)pp[i]-1;k=dihedrals[i][1]=(int)pp[i+ndihedrals]-1;dihparams[i][0]=pp[i+3*ndihedrals]/pp[i+2*ndihedrals]*pp[i+5*ndihedrals];dihparams[i][1]=pp[i+4*ndihedrals]*DTOR;dihparams[i][2]=pp[i+5*ndihedrals];
				i1=angles[j][0];i2=angles[j][1];i3=angles[k][0];i4=angles[k][1];if (i1==i3) {o1=i2;o2=i1;o3=i4;o4=-1;} else if (i1==i4) {o1=i2;o2=i1;o3=i3;o4=1;} else if (i2==i3) {o1=i1;o2=i2;o3=i4;o4=1;} else if (i2==i4) {o1=i1;o2=i2;o3=i3;o4=-1;} else mexErrMsgTxt("Dihedral is improper!");
				dihedrals[i][2]=o1;dihedrals[i][3]=o2;dihedrals[i][4]=o3;dihedrals[i][5]=o4;}
		}
		pm=mxGetField(prhs[3],0,"improper"); 
		if (!pm) nimproper=0; else {mexErrMsgTxt("We're now using the regular dihedral code for improper dihedrals!");
			size=mxGetDimensions(pm); if (size[1]!=5) mexErrMsgTxt("Impropers must have 5 columns!");
			nimproper=size[0]; pp=(double*)mxGetData(pm); if (nimproper>100000) mexErrMsgTxt("More than 100000 impropers is a bad idea!");
			improper=(int(*)[2])malloc(nimproper*sizeof(int[2]));
			impparams=(double(*)[3])malloc(nimproper*sizeof(double[3]));
			for (i=0;i<nimproper;i++) {improper[i][0]=(int)pp[i]-1;improper[i][1]=(int)pp[i+nimproper]-1;impparams[i][0]=pp[i+2*nimproper]*pp[i+4*nimproper];impparams[i][1]=pp[i+3*nimproper]*DTOR;impparams[i][2]=pp[i+4*nimproper];}
		}
		pm=mxGetField(prhs[3],0,"VDW");
		if (!pm) nVDW=0; else {
			size=mxGetDimensions(pm); if (size[1]!=5) mexErrMsgTxt("VDW must have 5 columns!");
			nVDW=size[0]; VDW=(XYZ*)malloc(nVDW*sizeof(XYZ)); VDW1=(int*)malloc(nVDW*sizeof(int)); VDW2=(int*)malloc(nVDW*sizeof(int));
			pp=(double*)mxGetData(pm);
			for (i=0;i<nVDW;i++) {VDW1[i]=(int)pp[i]-1; VDW2[i]=(int)pp[i+nVDW]-1; VDW[i].x=pp[i+2*nVDW]; VDW[i].y=pp[i+3*nVDW]; VDW[i].z=pp[i+4*nVDW];}
		}
		pm=mxGetField(prhs[3],0,"implicit");
		if (!pm) implicitwater=0; else {
			implicitwater=1;
			mxArray * pm2=mxGetField(pm,0,"charges"); if (!pm2) mexErrMsgTxt("Implicit water structure must specify charges!");
			if (nvert!=mxGetNumberOfElements(pm2)) mexErrMsgTxt("Implicit water charges must have one entry per atoms!");
			charges=(double*)malloc(nvert*sizeof(double)); pp=(double*)mxGetData(pm2);for (i=0;i<nvert;i++) charges[i]=pp[i];
			pm2=mxGetField(pm,0,"types"); if (!pm2) mexErrMsgTxt("Implicit water structure must specify atom types ('H','C','N','O','S')");
			if (nvert!=mxGetNumberOfElements(pm2)) mexErrMsgTxt("Implicit water atom types must have one entry per atom!");
			if (!mxIsChar(pm2)) mexErrMsgTxt("Implicit water atom types must be an array of characters ('H','C','N','O','S')");
			char *mtypes=mxArrayToString(pm2);
			atomtype=(int*)malloc(nvert*sizeof(int));
			for (i=0;i<nvert;i++)
				switch (toupper(mtypes[i])) {
				case 'H': atomtype[i]=0; break;
				case 'C': atomtype[i]=1; break;
				case 'N': atomtype[i]=2; break;
				case 'O': atomtype[i]=3; break;
				case 'S': atomtype[i]=4; break;
				default: mexErrMsgTxt("Unrecognized atom type!");
			}
			mxFree(mtypes);
			scratchi=(double*)malloc(nvert*5*sizeof(double));
		}
		doForce(NULL,0,zero,zero,0);
		pm=mxCreateString("TDxInput.Device");
		have3DInput=!mexCallMATLABWithTrap(1,&srv,1,&pm,"actxserver");//should also start the server!!! (for buttons)
		activexinput=have3DInput;
#ifdef LINUX_INPUT
		int err;
		if (!have3DInput) have3DInput=((err=spnav_open())==0); if (err) printf("Can't find 3d mouse on linux! Error code %d\n",err);
#endif
	}
	SliderSet(&sliders[0],&parms.fric);SliderSet(&sliders[1],&parms.temp);SliderSet(&sliders[2],&parms.tempcontrol);
	p2[0]=mxCreateDoubleScalar(0);p2[1]=mxCreateString("MonitorPositions");
	mexCallMATLAB(1,&pm,2,p2,"get"); int monitors=mxGetM(pm); pp=(double*)mxGetData(pm);
	int scx,scy;
#ifdef _WIN32
	scx=1+pp[2*monitors]-pp[0]; scy=1+pp[3*monitors]-pp[monitors];
#else
	scx=pp[2*monitors]; scy=pp[3*monitors];
#endif
	if (!camera.defaultdiaglength) camera.defaultdiaglength=14*25.4; //14 inch screen by default
	camera.diaglength=camera.defaultdiaglength; 
	camera.pixelsize=camera.diaglength/sqrt((double)scx*scx+scy*scy);
	camera.eyedist=camera.defaulteyedist=24*25.4; //viewer is expected to be 24 inches from screen by default
	camera.eyesep=camera.defaulteyesep=65.0;
	camera.viewheight=camera.defaultviewheight=sqrt((double)(nvert<10?10:nvert))*2;
	glutInit(&argc,argv);
	/* DVV - removed GLUT_ACCUM */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
//glutInitDisplayMode(... | GLUT_MULTISAMPLE);
glEnable(0x809D);//GL_MULTISAMPLE_ARB);
	/* Create the window and handlers */
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutCreateWindow("Proteins");
	camera.screenwidth = 400;
	camera.screenheight = 300;
	glutReshapeWindow(camera.screenwidth,camera.screenheight);
	glutDisplayFunc(HandleDisplay);
	glutReshapeFunc(HandleReshape);
	glutKeyboardFunc(HandleKeyboard);
	glutSpecialFunc(HandleSpecialKeyboard);
	glutMouseFunc(HandleMouse);
	glutMotionFunc(HandleMouseMotion);
	CreateEnvironment();
	CameraHome(0);
#ifdef _WIN32
	void *fp=wglGetProcAddress( "wglSwapIntervalEXT" );if (fp) ((BOOL (WINAPI *)( int ))fp)(0);//whether to wait for vsync
#endif
	/* Set up the model menu */
	modelmenu = glutCreateMenu(HandleModelMenu);
	sprintf(str,"Default: %.1f feet",camera.defaulteyedist/25.4/12);
	glutAddMenuEntry(str,0);
	glutAddMenuEntry("2 feet",1);
	glutAddMenuEntry("2.5 feet",2);
	glutAddMenuEntry("3 feet",3);
	glutAddMenuEntry("4 feet",4);

	/* Set up the glasses menu */
	glassesmenu = glutCreateMenu(HandleGlassesMenu);
	glutAddMenuEntry("Red-Blue",REDBLUE);
	glutAddMenuEntry("Red-Green",REDGREEN);
	glutAddMenuEntry("Red-Cyan",REDCYAN);
	glutAddMenuEntry("Blue-Red",BLUERED);
	glutAddMenuEntry("Green-Red",GREENRED);
	glutAddMenuEntry("Cyan-Red",CYANRED);

	outputmenu = glutCreateMenu(HandleOutputMenu);
	glutAddMenuEntry("Stress Tensor",1);
	glutAddMenuEntry("1000 Velocities",2);

	/* Set up the camera menu */
	cameramenu = glutCreateMenu(HandleCameraMenu);
	glutAddMenuEntry("Reset",1);
	glutAddMenuEntry("Bigger",2);
	glutAddMenuEntry("Smaller",3);
	glutAddMenuEntry("No Parallax",4);
	glutAddMenuEntry("Parallax",5);

/*	satmenu = glutCreateMenu(HandleSaturationMenu);
	glutAddMenuEntry("0.0",0);
	glutAddMenuEntry("0.2",1);
	glutAddMenuEntry("0.4",2);
	glutAddMenuEntry("0.6",3);
	glutAddMenuEntry("0.8",4);
	glutAddMenuEntry("1.0",5);*/

	ambmenu = glutCreateMenu(HandleAmbientMenu);
	glutAddMenuEntry("0.0",0);
	glutAddMenuEntry("0.1",1);
	glutAddMenuEntry("0.2",2);
	glutAddMenuEntry("0.3",3);
	glutAddMenuEntry("0.4",4);
	glutAddMenuEntry("0.5",5);

	difmenu = glutCreateMenu(HandleDiffuseMenu);
	glutAddMenuEntry("0.0",0);
	glutAddMenuEntry("0.1",1);
	glutAddMenuEntry("0.2",2);
	glutAddMenuEntry("0.3",3);
	glutAddMenuEntry("0.4",4);
	glutAddMenuEntry("0.5",5);

	specmenu = glutCreateMenu(HandleSpecularMenu);
	glutAddMenuEntry("0.0",0);
	glutAddMenuEntry("0.1",1);
	glutAddMenuEntry("0.2",2);
	glutAddMenuEntry("0.3",3);
	glutAddMenuEntry("0.4",4);
	glutAddMenuEntry("0.5",5);

	diagmenu = glutCreateMenu(HandleDiagMenu);
	sprintf(str,"Default: %.1f inches",camera.defaultdiaglength/25.4);
	glutAddMenuEntry(str,0);
	glutAddMenuEntry("11 inches",1);
	glutAddMenuEntry("15 inches",2);
	glutAddMenuEntry("19 inches",3);
	glutAddMenuEntry("24 inches",4);
	glutAddMenuEntry("30 inches",5);

	/* Set up the main menu */
	mainmenu = glutCreateMenu(HandleMainMenu);
	glutAddSubMenu("Distance to Screen",modelmenu);
	glutAddSubMenu("Screen Diagonal",diagmenu);
	glutAddSubMenu("Camera",cameramenu);
	glutAddSubMenu("Glasses Colors",glassesmenu);
//	glutAddSubMenu("Saturation",satmenu);
	glutAddSubMenu("Ambient Lighting",ambmenu);
	glutAddSubMenu("Diffuse Lighting",difmenu);
	glutAddSubMenu("Specular Lighting",specmenu);
	glutAddSubMenu("Output",outputmenu);
	glutAddMenuEntry("Quit",9);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

//	spherelist=glGenLists(12);
//	sphereSetup();
	if (writefile) {filepointer=fopen("physicsdata.dat","wb"); fprintf(filepointer,"NASN\n");}
	/* Ready to go! */
	running=1;
	HandleTimer(0);
	glutMainLoop();
}

/*
   This is where global OpenGL/GLUT settings are made, 
   that is, things that will not change in time 
*/
void CreateEnvironment(void)
{
   int num[2];
glEnable(0x8DB9);// FRAMEBUFFER_SRGB_EXT
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_LINE_SMOOTH);
   glDisable(GL_POINT_SMOOTH);
   glDisable(GL_POLYGON_SMOOTH); 
   glEnable(GL_NORMALIZE);
   glDisable(GL_DITHER);
   glDisable(GL_CULL_FACE);
   glDisable(GL_BLEND);    /* Not necessary but for bug in PS350 driver */
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glLineWidth(1.0);
   glPointSize(1.0);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   glFrontFace(GL_CCW);
   glClearColor(0.75,0.75,0.75,1.0);
   glClearColor(0.3,0.3,0.3,1.0);
   glClearAccum(0.0,0.0,0.0,0.0);   /* The default */
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

/*
   This is the basic display callback routine
   It creates the geometry, lighting, and viewing position
*/
void HandleDisplay(void)
{
   double d,nc,fc; //nc,fc are near and far clipping plane
   XYZ campos,right; COLOR black={0,0,0}; int i;
   d=camera.eyedist*camera.viewheight/camera.screenheight/camera.pixelsize;
   Normalize(&camera.vd);
   campos.x=camera.pr.x-camera.vd.x*d;
   campos.y=camera.pr.y-camera.vd.y*d;
   campos.z=camera.pr.z-camera.vd.z*d;
   nc=d/20; fc=d+200;

   /* Derive the the "right" vector */
   CROSSPROD(camera.vd,camera.vu,right);
   Normalize(&right);
   right.x *= camera.eyesep/camera.pixelsize / 2.0*(camera.viewheight/camera.screenheight);
   right.y *= camera.eyesep/camera.pixelsize / 2.0*(camera.viewheight/camera.screenheight);
   right.z *= camera.eyesep/camera.pixelsize / 2.0*(camera.viewheight/camera.screenheight);

   /* Set the buffer for writing and reading */
   glDrawBuffer(GL_BACK);
   glReadBuffer(GL_BACK);

   /* Clear things */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* Set projection */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(nc/camera.eyedist*camera.pixelsize*.5*(-camera.screenwidth+camera.eyesep/camera.pixelsize),
	   nc/camera.eyedist*camera.pixelsize*.5*(camera.screenwidth+camera.eyesep/camera.pixelsize),
	   nc/camera.eyedist*camera.pixelsize*.5*(-camera.screenheight),nc/camera.eyedist*camera.pixelsize*.5*(camera.screenheight),nc,fc);
   glViewport(0,0,camera.screenwidth,camera.screenheight);

   /* Left eye filter */
   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
   switch (glassestype) {
   case REDBLUE:
   case REDGREEN:
   case REDCYAN:
      glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
      break;
   case BLUERED:
      glColorMask(GL_FALSE,GL_FALSE,GL_TRUE,GL_TRUE);
      break;
   case GREENRED:
      glColorMask(GL_FALSE,GL_TRUE,GL_FALSE,GL_TRUE);
      break;
   case CYANRED:
      glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_TRUE);
      break;
   } 

   /* Create the model */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(campos.x - right.x,
             campos.y - right.y,
             campos.z - right.z,
             camera.pr.x-right.x,camera.pr.y-right.y,camera.pr.z-right.z,
             camera.vu.x,camera.vu.y,camera.vu.z);
   CreateWorld();
   glFlush();
   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); 

   glDrawBuffer(GL_BACK);

   glClear(GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(nc/camera.eyedist*camera.pixelsize*.5*(-camera.screenwidth-camera.eyesep/camera.pixelsize),nc/camera.eyedist*camera.pixelsize*.5*(camera.screenwidth-camera.eyesep/camera.pixelsize),
	   nc/camera.eyedist*camera.pixelsize*.5*(-camera.screenheight),nc/camera.eyedist*camera.pixelsize*.5*(camera.screenheight),nc,fc);
   glViewport(0,0,camera.screenwidth,camera.screenheight);

   /* Right eye filter */
   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
   switch (glassestype) {
   case REDBLUE:
      glColorMask(GL_FALSE,GL_FALSE,GL_TRUE,GL_TRUE);
      break;
   case REDGREEN:
      glColorMask(GL_FALSE,GL_TRUE,GL_FALSE,GL_TRUE);
      break;
   case REDCYAN:
      glColorMask(GL_FALSE,GL_TRUE,GL_TRUE,GL_TRUE);
      break;
   case BLUERED:
   case GREENRED:
   case CYANRED:
      glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
      break;
   } 

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(campos.x + right.x,
             campos.y + right.y,
             campos.z + right.z,
             camera.pr.x+right.x,camera.pr.y+right.y,camera.pr.z+right.z,
             camera.vu.x,camera.vu.y,camera.vu.z);
   CreateWorld();
   glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE); 
   if (allsliders.visible) RenderSliders(&allsliders);
   for (i=0;i<nstringstodraw;i++) DrawString(10,camera.screenheight-30-20*i,stringstodraw[i],black,0);
   if (doframerate) framerate();
   glFlush();

   /* Addin the new image and copy the result back */
   /* DVV
   glAccum(GL_ACCUM,1.0);
   glAccum(GL_RETURN,1.0);
   */
   
   /* Optionally dump image */
   if (windowdump) {
      WindowDump(camera.screenwidth,camera.screenheight);
      windowdump = FALSE;
   }

   /* Let's look at it */
   glutSwapBuffers(); 
}

/*
   Create one of the possible models
   Handle the rotation of the model, about the y axis
*/
void CreateWorld(void)
{
   COLOR col,colors[]={{1,1,1},{.1,.1,.1},{.4,0,1},{.2,.4,0},{.6,1,0},{.5,.5,.5},{1,1,1},{.1,.1,.1},{.4,0,1},{.2,.4,0},{.6,1,0},{.5,.5,.5}};
   static double ang=0;
   int i,ind,*inds=G_inds;
   double rs2[]={.15,.2,.25,.3,.35,.1,.15,.2,.25,.3,.35,.1},r;
   double ii=saturation,dum,rs[2]; rs[1]=.08*bigscale; rs[0]=.08*smallscale;
   for (i=0;i<6;i++) {rs2[i+6]*=smallscale; rs2[i]*=bigscale;}
   MakeLighting();
   for (i=0;i<nvert;i++) {
	   switch (toupper(types[i][0])) {
		   case 'H': ind=0; break;
		   case 'C': ind=1; break;
		   case 'N': ind=2; break;
		   case 'O': ind=3; break;
		   case 'S': ind=4; break;
		   default: myexitErr("Invalid atom type!"); break;
	   } 
	   if (atomdisplay==1) {col=colors[(charges[i]>0)?2:4]; r=pow(fabs(charges[i]),.8)*.5;}//.8th power is completely arbitrary; looks good
	   else if ((atomdisplay==2) && count) {col=colors[ind]; r=scratchi[i+nvert];}
	   else {col=colors[ind]; r=rs2[ind%6];} 
	   if ((drawbig[i] || allbig) && (!allsmall)) MakeSphere(verts[i],r,col);
	   else {MakeBands(verts[i],r/bigscale*smallscale,col,15,r/bigscale*smallscale*.15); ind+=6;}
	   inds[i]=ind;
   }
   for (i=0;i<nedge;i++) MakeCylinder(verts[edges[i][0]],verts[edges[i][1]],rs[(drawbig[edges[i][0]] && drawbig[edges[i][1]])],15,colors[5]);
   if (showcursor) {MakeOctahedron(cursorpos,.8,colors[5],ang); ang+=4; if (ang>360) ang-=360;}
   if (npush) for (i=0;i<npush;i++) MakeBands(verts[push[i]],.05,colors[5],15,rs2[inds[push[i]]]*1.2+.05);
   if (nglued) for (i=0;i<nglued;i++) MakeBands(verts[glued[i]],rs2[inds[glued[i]]]+.1,colors[5],15,.05);
   if (displayforces && count) for(i=0;i<nvert;i++) {
	   XYZ mid,end,f; f=(displayforces==3)?p_Fimplicit2[i]:(displayforces==2)?p_Fimplicit1[i]:(singleatomforces!=-1)?p_Fsingle[i]:p_Fvdw[i];
	   if (displayforces==2) {f.x=p_Fvdw[i].x+p_Fimplicit1[i].x;f.y=p_Fvdw[i].y+p_Fimplicit1[i].y;f.z=p_Fvdw[i].z+p_Fimplicit1[i].z;}
	   double mag=MODULUS(f),o=.2,sc=o*.7; if (mag>1e-10) {if ((o-sc)*mag<.25) sc=o-.25/mag; if (sc<0) sc=0;}
	   COLOR arrowcolor[3]={{.2,.4,0},{.6,1,0},{.1,.1,.1}};
	   end.x=verts[i].x+o*f.x;end.y=verts[i].y+o*f.y;end.z=verts[i].z+o*f.z;
	   mid.x=verts[i].x+sc*f.x;mid.y=verts[i].y+sc*f.y;mid.z=verts[i].z+sc*f.z;
	   MakeCylinder(verts[i],mid,.08,15,arrowcolor[displayforces-1]);MakeCone(mid,end,MIN(.35,mag*o),15,arrowcolor[displayforces-1]);
   }
}

void MakeOctahedron(XYZ p,double r,COLOR col,double ang) {
	double s=sin(ang*DTOR),c=cos(ang*DTOR);
	GLfloat black[]={0,0,0,1},white[]={1,1,1,1};
	glShadeModel(GL_FLAT);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,black);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor3f(col.r,col.g,col.b);
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0,0,1);
	glVertex3f(p.x,p.y,p.z+r);
	glNormal3f(s,c,0);
	glVertex3f(p.x+r*s,p.y+r*c,p.z);
	glNormal3f(-c,s,0);
	glVertex3f(p.x-r*c,p.y+r*s,p.z);
	glNormal3f(-s,-c,0);
	glVertex3f(p.x-r*s,p.y-r*c,p.z);
	glNormal3f(c,-s,0);
	glVertex3f(p.x+r*c,p.y-r*s,p.z);
	glNormal3f(s,c,0);
	glVertex3f(p.x+r*s,p.y+r*c,p.z);
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0,0,-1);
	glVertex3f(p.x,p.y,p.z-r);
	glNormal3f(s,c,0);
	glVertex3f(p.x+r*s,p.y+r*c,p.z);
	glNormal3f(c,-s,0);
	glVertex3f(p.x+r*c,p.y-r*s,p.z);
	glNormal3f(-s,-c,0);
	glVertex3f(p.x-r*s,p.y-r*c,p.z);
	glNormal3f(-c,s,0);
	glVertex3f(p.x-r*c,p.y+r*s,p.z);
	glNormal3f(s,c,0);
	glVertex3f(p.x+r*s,p.y+r*c,p.z);
	glEnd();
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
	glShadeModel(GL_SMOOTH);
}

void MakeBands(XYZ p,double r,COLOR c,int n,double w) {
	XYZ s,e;
	s=e=p;s.x+=w; e.x-=w; MakeCylinder(s,e,r,n,c);
	s=e=p;s.y+=w; e.y-=w; MakeCylinder(s,e,r,n,c);
	s=e=p;s.z+=w; e.z-=w; MakeCylinder(s,e,r,n,c);
}

void MakeBox(void)
{
   XYZ pmin = {-3,-3,-3},pmax = {3,3,3};

   glColor3f(1.0,1.0,1.0);
   glBegin(GL_LINE_STRIP);
   glVertex3f(pmin.x,pmin.y,pmin.z);
   glVertex3f(pmax.x,pmin.y,pmin.z);
   glVertex3f(pmax.x,pmin.y,pmax.z);
   glVertex3f(pmin.x,pmin.y,pmax.z);
   glVertex3f(pmin.x,pmin.y,pmin.z);
   glVertex3f(pmin.x,pmax.y,pmin.z);
   glVertex3f(pmax.x,pmax.y,pmin.z);
   glVertex3f(pmax.x,pmax.y,pmax.z);
   glVertex3f(pmin.x,pmax.y,pmax.z);
   glVertex3f(pmin.x,pmax.y,pmin.z);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(pmax.x,pmin.y,pmin.z); glVertex3f(pmax.x,pmax.y,pmin.z);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(pmax.x,pmin.y,pmax.z); glVertex3f(pmax.x,pmax.y,pmax.z);
   glEnd();
   glBegin(GL_LINES);
   glVertex3f(pmin.x,pmin.y,pmax.z); glVertex3f(pmin.x,pmax.y,pmax.z);
   glEnd();
}

void MakeCylinder(XYZ start,XYZ end,double r,int n,COLOR color)
{
   XYZ e,p,n1, n2,d,a={.1230498,.983527,.4050923};//a is basically random
   int i;
   double t,ct,st;
   d.x=end.x-start.x; d.y=end.y-start.y; d.z=end.z-start.z;
   CROSSPROD(a,d,n1); Normalize(&n1);
   CROSSPROD(d,n1,n2); Normalize(&n2);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   glColor3f(color.r,color.g,color.b);
   glBegin(GL_QUAD_STRIP);
   for (i=0;i<=n;i++) {
      t = i * TWOPI / n; ct=cos(t); st=-sin(t);
      e.x = n1.x * ct + n2.x * st;
      e.y = n1.y * ct + n2.y * st;
      e.z = n1.z * ct + n2.z * st;
      p.x = start.x + r * e.x;
      p.y = start.y + r * e.y;
      p.z = start.z + r * e.z;
      glNormal3f(e.x,e.y,e.z);
//      glTexCoord2f(i/(double)n,2*j/(double)n);
      glVertex3f(p.x,p.y,p.z);

      p.x = end.x + r * e.x;
      p.y = end.y + r * e.y;
      p.z = end.z + r * e.z;

	  glNormal3f(e.x,e.y,e.z);
//      glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
      glVertex3f(p.x,p.y,p.z);

   }
   glEnd();
}

void MakeCone(XYZ start,XYZ end,double r,int n,COLOR color)
{
	XYZ e,p,n1,sh, n2,d,a={.1230498,.983527,.4050923};//a is basically random
	int i;
	double t,ct,st,dr,scale;
	d.x=end.x-start.x; d.y=end.y-start.y; d.z=end.z-start.z,dr=MODULUS(d);
	if (dr>1e-10) {
		double temp=1.0/sqrt(r*r+dr*dr), temp2=r*temp/dr;sh.x=d.x*temp2;sh.y=d.y*temp2;sh.z=d.z*temp2;scale=dr*temp;
		CROSSPROD(a,d,n1); Normalize(&n1);
		CROSSPROD(d,n1,n2); Normalize(&n2);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glColor3f(color.r,color.g,color.b);
		glBegin(GL_QUAD_STRIP);
		for (i=0;i<=n;i++) {
			t = i * TWOPI / n; ct=cos(t); st=-sin(t);
			e.x = n1.x * ct + n2.x * st;
			e.y = n1.y * ct + n2.y * st;
			e.z = n1.z * ct + n2.z * st;
			p.x = start.x + r * e.x;
			p.y = start.y + r * e.y;
			p.z = start.z + r * e.z;
			e.x=e.x*scale+sh.x;
			e.y=e.y*scale+sh.y;
			e.z=e.z*scale+sh.z;
			glNormal3f(e.x,e.y,e.z);
			//      glTexCoord2f(i/(double)n,2*j/(double)n);
			glVertex3f(p.x,p.y,p.z);

			p.x = end.x + 0 * e.x;
			p.y = end.y + 0 * e.y;
			p.z = end.z + 0 * e.z;

			glNormal3f(e.x,e.y,e.z);
			//      glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
			glVertex3f(p.x,p.y,p.z);

		}
		glEnd();
	}
}

void MakeSphere(XYZ c,double r,COLOR color)
{
	if (!sphereinit) {
		int n=40;//resolution of the sphere, used to be a parameter!
		sphereinit=1;
		spherelist=glGenLists(1);
		glNewList(spherelist,GL_COMPILE);
		int i,j,ind=0;
		float t1,t2,t3;
		XYZ e,p;
		if (n>99) myexitErr("Cannot draw spheres with more than 5000 faces!");
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//		glColor3f(color.r,color.g,color.b);
		for (j=0;j<n/2;j++) {//I've only got spheres of one resolution
			t1 = -PID2 + j * PI / (n/2);
			t2 = -PID2 + (j + 1) * PI / (n/2);
			for (i=0;i<=n;i++) {
				t3 = i * TWOPI / n;
				sphereP[ind][0]=cos(t1)*cos(t3);
				sphereP[ind][1]=sin(t1);
				sphereP[ind][2]=cos(t1)*sin(t3);
				ind++;
				sphereP[ind][0]=cos(t2)*cos(t3);
				sphereP[ind][1]=sin(t2);
				sphereP[ind][2]=cos(t2)*sin(t3);
				ind++;
			}
		}
		ind=0;
		for (j=0;j<n/2;j++) {
			glBegin(GL_QUAD_STRIP);
			for (i=0;i<=n;i++) {
				glNormal3fv(sphereP[ind]);
				glVertex3f(sphereP[ind][0],sphereP[ind][1],sphereP[ind][2]);
				ind++;
				glNormal3fv(sphereP[ind]);
				glVertex3f(sphereP[ind][0],sphereP[ind][1],sphereP[ind][2]);
				ind++;
			}
			glEnd();
		}
		glEndList();
	}
	glColor3f(color.r,color.g,color.b);
	glPushMatrix();
	glTranslated(c.x,c.y,c.z);
	glScaled(r,r,r);
	glCallList(spherelist);
	glPopMatrix();
}
/*
   Set up the lighing environment
*/
void MakeLighting(void)
{
   GLfloat fullambient[4],diffuse[4],specular[4];// = {.20,.20,.20,1.0};
   GLfloat position[4] = {-700.0,1000.0,1200.0,0.0};
   GLfloat position2[4] = {700.0,-1000.0,-1200.0,0.0};
   GLfloat white[4]={1,1,1,1};
//   GLfloat ambient[4]  = {0.0,0.0,0.0,1.0};
//   GLfloat diffuse[4]  = {1.0,1.0,1.0,1.0};
//   GLfloat specular[4] = {1.0,1.0,1.0,1.0};
//   GLfloat dark[4] = {.1,.1,.1,1};
//   GLfloat gray[4] = {.3,.3,.3,1};
   fullambient[0]=fullambient[1]=fullambient[2]=ambientstrength; fullambient[3]=1;
   diffuse[0]=diffuse[1]=diffuse[2]=diffusestrength; diffuse[3]=1;
   specular[0]=specular[1]=specular[2]=specularstrength; specular[3]=1;

   /* Turn off all the lights */
   glDisable(GL_LIGHT0);
   glDisable(GL_LIGHT1);
   glDisable(GL_LIGHT2);
   glDisable(GL_LIGHT3);
   glDisable(GL_LIGHT4);
   glDisable(GL_LIGHT5);
   glDisable(GL_LIGHT6);
   glDisable(GL_LIGHT7);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,TRUE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
//   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
   /* Turn on the appropriate lights */
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT,fullambient);
   glLightfv(GL_LIGHT0,GL_POSITION,position);
//   glLightfv(GL_LIGHT0,GL_AMBIENT,dark);
   glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT1,GL_POSITION,position2);
//   glLightfv(GL_LIGHT1,GL_AMBIENT,dark);
   glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse);
   glLightfv(GL_LIGHT1,GL_SPECULAR,specular);
   glEnable(GL_LIGHT1);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,30.0);
   /* Sort out the shading algorithm */
   glShadeModel(GL_SMOOTH);

   /* Turn lighting on */
   glEnable(GL_LIGHTING);
}

int getClosest(XYZ loc,int *avoid,int navoid) {
	int i,mi=-1; double d,m=1e100;
	for (i=0;i<nvert;i++) {
		d=(loc.x-verts[i].x)*(loc.x-verts[i].x)+(loc.y-verts[i].y)*(loc.y-verts[i].y)+(loc.z-verts[i].z)*(loc.z-verts[i].z);
		if (d<m) {int good=1; for (int j=0;j<navoid;j++) good&=(avoid[j]!=i); if (good) {m=d; mi=i;}}
	}
	return mi;
}

/*
   Deal with plain key strokes
*/
void HandleKeyboard(unsigned char key,int x, int y)
{
   int i,dup,mi=0,shift; double *p,d;//,m=1000000;
   switch (key) {
   case 'p': case 'P': allsliders.visible=!allsliders.visible; break;
   case 13: shift=glutGetModifiers()==GLUT_ACTIVE_SHIFT;//"enter"
	   if (!showcursor) {
		   showcursor=1;
		   if (!shift) {cursorpos=verts[push[0]]; npush=0;}
	   } else {
		   mi=getClosest(cursorpos,push,npush);
		   if (shift || npush==0) {if (mi!=-1) {push[npush++]=mi; if ((npush==1) && (singleatomforces>=0)) singleatomforces=mi;}else showcursor=0;}
		   if (!shift) showcursor=0;
	   }
   break;
   case 'b':
   case 'B':
	   if (allbig) {allbig=0; allsmall=1;} else if (allsmall) {allsmall=0; allbig=0;} else {allbig=1;allsmall=0;}
	   break;
   case 'g': if (nglued) nglued=0; else if (showcursor) {
	   mi=getClosest(cursorpos,glued,nglued);
		nglued=1; glued[0]=mi; p_V[mi].x=p_V[mi].y=p_V[mi].z=0;
		} else if (glued[0]!=-1) {nglued=1; mi=glued[0]; p_V[mi].x=p_V[mi].y=p_V[mi].z=0;} break;
   case 'G': if (showcursor) {mi=getClosest(cursorpos,glued,nglued); dup=0;
	   for (i=0;i<nglued;i++) if (glued[i]==mi) dup=1;
	   if (!dup) {glued[nglued++]=mi; p_V[mi].x=p_V[mi].y=p_V[mi].z=0;}} break;
   case ' ': pause=!pause; break;
   case ESC:                            /* Quit */
   case 'Q':
   case 'q': 
	   myexit();
      break;
   case 'h':                           /* Go home     */
   case 'H':
      CameraHome(0);
      break;
   case 'c':
   case 'C':
	   atomdisplay=(atomdisplay+1)%(1+implicitwater*(1+(count>0)));
	   break;
   case 'w': //toggle force vectors
   case 'W':
	   displayforces=(displayforces+1)%(2+2*implicitwater); if (displayforces>1) singleatomforces=-1;
	   break;
   case 's'://single atom force
   case 'S':
	   if (count) {if (singleatomforces>=0) singleatomforces=-1; else if (npush) {singleatomforces=push[0]; displayforces=1;}}
	   break;
//   case 'w':                           /* Save one image */
//   case 'W':
//      windowdump = TRUE;
//      break;
   case '[':                           /* Roll anti clockwise */
      RotateCamera(0,0,-1);
      break;
   case ']':                           /* Roll clockwise */
      RotateCamera(0,0,1);
      break;
   case 'f': case 'F': doframerate=!doframerate; break;
   }
}

/*
   Deal with special key strokes
*/
void HandleSpecialKeyboard(int key,int x, int y)
{
   switch (key) {
   case GLUT_KEY_LEFT:
      RotateCamera(-1,0,0);
      break;
   case GLUT_KEY_RIGHT:
      RotateCamera(1,0,0);
      break;
   case GLUT_KEY_UP:
      RotateCamera(0,1,0);
      break;
   case GLUT_KEY_DOWN:
      RotateCamera(0,-1,0);
      break;
   }
}

/*
   Rotate (ix,iy) or roll (iz) the camera about the focal point
   ix,iy,iz are flags, 0 do nothing, +- 1 rotates in opposite directions
   Correctly updating all camera attributes
*/
void RotateCamera(double ix,double iy,double iz)
{
   XYZ vp,vu,vd;
   XYZ right;
   XYZ newvp,newr;
   double radius,dd,radians;
   double dx,dy,dz;

   vu = camera.vu;
   Normalize(&vu);
   vp = camera.vp;
   vd = camera.vd;
   Normalize(&vd);
   CROSSPROD(vd,vu,right);
   Normalize(&right);
   radians = dtheta * PI / 180.0;

   /* Handle the roll */
   if (iz != 0) {
      camera.vu.x += iz * right.x * radians;
      camera.vu.y += iz * right.y * radians;
      camera.vu.z += iz * right.z * radians;
      Normalize(&camera.vu);
      return;
   }

   /* Distance from the rotate point */
   dx = camera.vp.x - camera.pr.x;
   dy = camera.vp.y - camera.pr.y;
   dz = camera.vp.z - camera.pr.z;
   radius = sqrt(dx*dx + dy*dy + dz*dz);

   /* Determine the new view point */
   dd = radius * radians;
   newvp.x = vp.x + dd * ix * right.x + dd * iy * vu.x - camera.pr.x;
   newvp.y = vp.y + dd * ix * right.y + dd * iy * vu.y - camera.pr.y;
   newvp.z = vp.z + dd * ix * right.z + dd * iy * vu.z - camera.pr.z;
   Normalize(&newvp);
   camera.vp.x = camera.pr.x + radius * newvp.x;
   camera.vp.y = camera.pr.y + radius * newvp.y;
   camera.vp.z = camera.pr.z + radius * newvp.z;

   /* Determine the new right vector */
   newr.x = camera.vp.x + right.x - camera.pr.x;
   newr.y = camera.vp.y + right.y - camera.pr.y;
   newr.z = camera.vp.z + right.z - camera.pr.z;
   Normalize(&newr);
   newr.x = camera.pr.x + radius * newr.x - camera.vp.x;
   newr.y = camera.pr.y + radius * newr.y - camera.vp.y;
   newr.z = camera.pr.z + radius * newr.z - camera.vp.z;

   camera.vd.x = camera.pr.x - camera.vp.x;
   camera.vd.y = camera.pr.y - camera.vp.y;
   camera.vd.z = camera.pr.z - camera.vp.z;
   Normalize(&camera.vd);

   /* Determine the new up vector */
   CROSSPROD(newr,camera.vd,camera.vu);
   Normalize(&camera.vu);
}

/*
   Handle mouse events
   Right button events are passed to menu handlers
*/
void HandleMouse(int button,int state,int x,int y)
{
	SLIDERCLICK s;
	if (state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
			s=IsOnSlider(x,y,&allsliders);
			if (s.which!=-1) HandleSlider(MOUSE_DOWN,&s,x,y,&allsliders); else
				{xlast=x;ylast=y;mousedragmeaning=MOUSE_ROTXY;}
		} else if (button == GLUT_MIDDLE_BUTTON) {
			mousedragmeaning=MOUSE_ROTZ; xlast=x;ylast=y; //does this work??? (worried about middle button elsewhere)
		} 
	} else if (mousedragmeaning==MOUSE_SLIDER) HandleSlider(MOUSE_UP,NULL,x,y,&allsliders); else mousedragmeaning=MOUSE_NONE;
}

/*
   Handle the main menu
*/
void HandleMainMenu(int whichone)
{
   switch (whichone) {
   case 9: 
	   myexit();
      break;
   }
}

/*
   Handle the rotation speed menu
   It is in degrees
*/
void HandleOutputMenu(int whichone)
{
	char names[][10]={"a","b","p","v","f","GGB"};
	const char *fields1[]={names[0],names[1],names[2]}, *fields2[]={names[2],names[3],names[4],names[5]}; 
	mxArray* pm;
	int i; int dims[]={nvert,3,noutput};double *p;
	mxDestroyArray(M_outp); nouts=1000000000; //inelegant way of disabling further output
	if (whichone==1) {
		G_plhs[0]=mxCreateStructMatrix(1,1,3,fields1);
		mxSetFieldByNumber(G_plhs[0],0,0,eigenSetup(1e-7));
		mxSetFieldByNumber(G_plhs[0],0,1,eigenSetup(1e-3)); 
		p=(double*)mxGetData(pm=mxCreateDoubleMatrix(nvert,3,mxREAL));
		for (i=0;i<nvert;i++) {p[i]=verts[i].x;p[i+nvert]=verts[i].y;p[i+2*nvert]=verts[i].z;};
		mxSetFieldByNumber(G_plhs[0],0,2,pm);
		myexit();
	} else if (whichone==2) {
		G_plhs[0]=mxCreateStructMatrix(1,1,4,fields2);
		mxSetFieldByNumber(G_plhs[0],0,0,pm=mxCreateNumericArray(3,dims,mxDOUBLE_CLASS,mxREAL));
		out_P=(double*)mxGetData(pm);
		mxSetFieldByNumber(G_plhs[0],0,1,pm=mxCreateNumericArray(3,dims,mxDOUBLE_CLASS,mxREAL));
		out_V=(double*)mxGetData(pm);
		mxSetFieldByNumber(G_plhs[0],0,2,pm=mxCreateNumericArray(3,dims,mxDOUBLE_CLASS,mxREAL));
		out_F=(double*)mxGetData(pm);
		mxSetFieldByNumber(G_plhs[0],0,3,pm=mxCreateDoubleMatrix(noutput,10,mxREAL));
		Energies=(double*)mxGetData(pm);
		outputcount=0;
	}
}

/*
   Handle the glasses type menu
*/
void HandleGlassesMenu(int whichone)
{
   glassestype = whichone;
}

/*
   Handle the camera menu
*/
void HandleCameraMenu(int whichone)
{
   CameraHome(whichone);   
}

void HandleSaturationMenu(int whichone) {
	switch (whichone) {
		case 0: saturation=0.0; break;
		case 1: saturation=0.2; break;
		case 2: saturation=0.4; break;
		case 3: saturation=0.6; break;
		case 4: saturation=0.8; break;
		case 5: saturation=1.0; break;
	}
}
void HandleAmbientMenu(int whichone) {
	switch (whichone) {
		case 0: ambientstrength=0.0; break;
		case 1: ambientstrength=0.1; break;
		case 2: ambientstrength=0.2; break;
		case 3: ambientstrength=0.3; break;
		case 4: ambientstrength=0.4; break;
		case 5: ambientstrength=0.5; break;
	}
}
void HandleDiffuseMenu(int whichone) {
	switch (whichone) {
		case 0: diffusestrength=0.0; break;
		case 1: diffusestrength=0.1; break;
		case 2: diffusestrength=0.2; break;
		case 3: diffusestrength=0.3; break;
		case 4: diffusestrength=0.4; break;
		case 5: diffusestrength=0.5; break;
	}
}
void HandleSpecularMenu(int whichone) {
	switch (whichone) {
		case 0: specularstrength=0.0; break;
		case 1: specularstrength=0.1; break;
		case 2: specularstrength=0.2; break;
		case 3: specularstrength=0.3; break;
		case 4: specularstrength=0.4; break;
		case 5: specularstrength=0.5; break;
	}
}

/*
   Handle the model menu
*/
void HandleModelMenu(int whichone)
{
	double d;
	switch (whichone) {
		case 0: camera.eyedist=camera.defaulteyedist; break;
		case 1: camera.eyedist=2.0*25.4*12; break;
		case 2: camera.eyedist=2.5*25.4*12; break;
		case 3: camera.eyedist=3.0*25.4*12; break;
		case 4: camera.eyedist=4.0*25.4*12; break;
	}
}

void HandleDiagMenu(int whichone)
{
	double d=camera.diaglength;
	switch (whichone) {
		case 0: camera.diaglength=camera.defaultdiaglength; break;
		case 1: camera.diaglength=11*25.4; break;
		case 2: camera.diaglength=15*25.4; break;
		case 3: camera.diaglength=19*25.4; break;
		case 4: camera.diaglength=24*25.4; break;
		case 5: camera.diaglength=30*25.4; break;
	}
	camera.pixelsize*=camera.diaglength/d;
}

/*
   What to do on an timer event
*/
void HandleTimer(int value)
{
	int i; XYZ avg;
	if (running) {
		glutPostRedisplay();
		if (have3DInput) do3DInput();
		if (physicsOn && !pause) {
			for (i=0;i<simiters;i++) 
				if (running) {
					if (count%5==0) recenter();
					doenergies=(outputcount!=-1);
					if (doenergies || (singleatomforces!=-1)) doPhysics<1>(); else doPhysics<0>();
				}
		}
		glutTimerFunc(0,HandleTimer,0);
	}
}
//note: one unit of our velocity corresponds to sqrt(1000*(1kCal->Joules)/simspeed) m/s, where 1000 is conversion from atomic mass units to avogadro's constant
//one unit of our time is (1Angstrom->meters)/(above speed); for simspeed=.0001, speed ~ 200000m/s and time ~ .5fs
void recenter() {
	double w,lL,tw=0,ke=0,I=0,a,b,fric_L,fric_R,boltzmann=.0019872*simspeed;//boltzmann's constant*avogadro constant/1kCal
	XYZ avg={0,0,0},v={0,0,0},L={0,0,0},Ln,temp,*iv=G_iv,chunkP={0,0,0};
	int i,j,chunki=0,chunks=0,chunksize=15; double chunkM=0,chunkE=0;
	int *gluet=G_gluet;
	memset(gluet,0,nvert*sizeof(int)); for (i=0;i<nglued;i++) gluet[glued[i]]=1;
//	for (i=0;i<nglued;i++) {j=glued[i]; p_V[j].x=p_V[j].y=p_V[j].z=0;}
	justrecentered=1;
	for (i=0;i<nvert;i++) {
		tw+=w=simspeed/weighti[i]; avg.x+=w*verts[i].x;avg.y+=w*verts[i].y;avg.z+=w*verts[i].z;
		v.x+=w*p_V[i].x;v.y+=w*p_V[i].y;v.z+=w*p_V[i].z;
		CROSSPROD(verts[i],p_V[i],temp); L.x+=w*temp.x;L.y+=w*temp.y;L.z+=w*temp.z;
	}
	avg.x/=tw;avg.y/=tw;avg.z/=tw; v.x/=tw;v.y/=tw;v.z/=tw; CROSSPROD(avg,v,temp);
	L.x-=temp.x*tw;L.y-=temp.y*tw;L.z-=temp.z*tw;
	lL=sqrt(L.x*L.x+L.y*L.y+L.z*L.z);if (lL!=0) {Ln.x=L.x/lL;Ln.y=L.y/lL;Ln.z=L.z/lL;} else Ln=L;
	for (i=0;i<nvert;i++) {
		w=simspeed/weighti[i];
		if (!angmomentum) {		verts[i].x-=avg.x;verts[i].y-=avg.y;verts[i].z-=avg.z; }
		CROSSPROD(Ln,verts[i],iv[i]);
		I+=w*(iv[i].x*iv[i].x+iv[i].y*iv[i].y+iv[i].z*iv[i].z);
		ke+=w*(p_V[i].x*p_V[i].x+p_V[i].y*p_V[i].y+p_V[i].z*p_V[i].z);
		chunkP.x+=w*p_V[i].x;chunkP.y+=w*p_V[i].y;chunkP.z+=w*p_V[i].z;chunkM+=w;chunki++;
		if (chunki==chunksize) {chunks++;chunki=0;chunkE+=(chunkP.x*chunkP.x+chunkP.y*chunkP.y+chunkP.z*chunkP.z)/chunkM;
			chunkM=chunkP.x=chunkP.y=chunkP.z=0;}
	}
	fric_L=.001;fric_R=.001;
	if (outputcount<=0) sprintf(stringstodraw[0],"KE: %.3g",ke); else sprintf(stringstodraw[0],"PE: %.5g",Energies[outputcount+noutput-1]+Energies[outputcount+noutput*2-1]+Energies[outputcount+noutput*3-1]+Energies[outputcount+noutput*4-1]+Energies[outputcount+noutput*5-1]+Energies[outputcount+noutput*6-1]);
	if (I!=0) ke-=tw*(v.x*v.x+v.y*v.y+v.z*v.z)+lL*lL/I;
	a=((ke==0) || (nglued>=nvert-2))?1:(sqrtf(parms.temp*.5*boltzmann*(3*(nvert-nglued)-6)/(.5*ke))-1)*parms.tempcontrol+1;
//	if (ke==0) a=1; else a=parms.temp*.5*boltzmann*(3*(nvert-nglued)-6)/(.5*ke);
//	a=(a-1)*parms.tempcontrol+1;
	sprintf(stringstodraw[1],"T: %.3g",(.5*ke)/(.5*boltzmann*(3*(nvert-nglued)-6)));
	sprintf(stringstodraw[2],"T adjust: %.3g",a-1); 
	if (chunks) {sprintf(stringstodraw[3],"Chunk T: %.3g",(.5*chunkE)/(1.5*boltzmann*chunks)); nstringstodraw=5;}
	else nstringstodraw=4;
	sprintf(stringstodraw[nstringstodraw-1],"V caps: %d",nvfix);
	if (I!=0) b=-(a-1+fric_R)*lL/I; else b=0;
	v.x=v.x*(a-1+fric_L);v.y=v.y*(a-1+fric_L);v.z=v.z*(a-1+fric_L);
	if (angmomentum) v.x=v.y=v.z=b=0;
	for (i=0;i<nvert;i++) {
		if (!gluet[i]) {p_V[i].x=p_V[i].x*a+iv[i].x*b-v.x;p_V[i].y=p_V[i].y*a+iv[i].y*b-v.y;p_V[i].z=p_V[i].z*a+iv[i].z*b-v.z;}//###
	}
}
/*
   Handle a window reshape/resize
*/
void HandleReshape(int w,int h)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glViewport(0,0,(GLsizei)w,(GLsizei)h);
   camera.screenwidth = w;
   camera.screenheight = h;
}

/*
   Move the camera to the home position 
   Or to a predefined stereo configuration
   The model is assumed to be in a 10x10x10 cube
   Centered at the origin
*/
void CameraHome(int mode)
{

   switch (mode) {
   case 0: camera.eyedist=camera.defaulteyedist;
   case 1:
      camera.eyesep=camera.defaulteyesep;
	  camera.viewheight=camera.defaultviewheight;//20;
   camera.aperture = 60;
   camera.pr = origin;

   camera.vd.x = 1;
   camera.vd.y = 0;
   camera.vd.z = 0;

   camera.vu.x = 0;
   camera.vu.y = 1;
   camera.vu.z = 0;

   camera.vp.x = -10;
   camera.vp.y = 0;
   camera.vp.z = 0;
	  break;
   case 2:
	   camera.viewheight/=1.4;
	   break;
   case 3:
	   camera.viewheight*=1.4;
	   break;
   case 4:
	   camera.eyesep=0; break;
   case 5:
       camera.eyesep=camera.defaulteyesep;
	   break;
   }
}

/*
   Write the current view to an image file
*/
int WindowDump(int width,int height)
{
   FILE *fptr;
   static int counter = 0;
   int status = TRUE;
   char fname[32];
   char *image;

   /* Allocate our buffer for the image */
   if ((image = (char*)malloc(3*width*height*sizeof(char))) == NULL) {
      fprintf(stderr,"WindowDump - Failed to allocate memory for image\n");
      return(FALSE);
   }

   glPixelStorei(GL_PACK_ALIGNMENT,1); /* Just in case */

   sprintf(fname,"folding_%04d.ppm",counter++);
   if ((fptr = fopen(fname,"w")) == NULL) {
      fprintf(stderr,"WindowDump - Failed to open file for window dump\n");
      return(FALSE);
   }

   /* Copy the image into our buffer */
   glReadBuffer(GL_BACK);
   glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);

   /* Write the file */
   fprintf(fptr,"P6\n%d %d\n255\n",width,height);
   if (fwrite(image,3*width*height*sizeof(char),1,fptr) != 1) 
      status = FALSE;
   fclose(fptr);

   free(image);
   return(status);
}

/*
   Handle mouse motion
*/
void HandleMouseMotion(int x,int y)
{
   double dx,dy;

   dx = x - xlast;
   dy = y - ylast;

   if (mousedragmeaning == MOUSE_ROTXY)
      RotateCamera(-dx/2,dy/2,0);
   else if (mousedragmeaning == MOUSE_ROTZ)
      RotateCamera(0,0,dx/2);// not dealing with motion-resetting correctly here???
   else if (mousedragmeaning == MOUSE_SLIDER)
	   HandleSlider(MOUSE_MOVE,NULL,x,y,&allsliders);

   xlast = x;
   ylast = y;
}

SLIDERCLICK IsOnSlider(double x,double y,SLIDERCONTAINER *sliders) {
	SLIDERCLICK c={-1,0}; int i; double sx,sy,r=sliders->sliderR;
	if (sliders->visible) {
		for (i=0;i<sliders->nsliders;i++) {
			sx=sliders->startx; sx+=(sx<0)*camera.screenwidth+sliders->spacingx*i; sy=sliders->starty; sy+=(sy<0)*camera.screenheight+sliders->spacingy*i;
			if ((x>=sx-r) && (y>=sy-r) && (x<=sx+sliders->width+r) && (y<sy+r)) {c.which=i; c.dir=-1+2*(x>sx+sliders->sliders[i].loc);}
			sx+=sliders->sliders[i].loc; sy+=(double)sliders->height/2; 
			if (sqrt((sx-x)*(sx-x)+(sy-y)*(sy-y))<=sliders->hotspotR) {c.which=i; c.dir=0;}
		}
	}
	return c;
}
void HandleSlider(int action,SLIDERCLICK *click,int x,int y,SLIDERCONTAINER *sliders) {
	SLIDER *slider=NULL; double rel;
	if (action==MOUSE_UP) mousedragmeaning=MOUSE_NONE;
	else if (action==MOUSE_DOWN) {
		slider=&sliders->sliders[sliders->current=click->which];
		if (click->dir==0) {sliders->clickref=x-slider->loc; mousedragmeaning=MOUSE_SLIDER;}
		else slider->loc=MAX(0,MIN(sliders->width,slider->loc+click->dir*sliders->jumppixels));
	} else (slider=&sliders->sliders[sliders->current])->loc=MAX(0,MIN(sliders->width,x-sliders->clickref));
	if (slider) {rel=slider->loc/sliders->width;
		if (slider->isLogarithmic<0) *slider->value=rel<slider->midfrac ? slider->start+rel/slider->midfrac*(slider->mid-slider->start) : exp(log(slider->mid)+(rel-slider->midfrac)/(1-slider->midfrac)*log(slider->end/slider->mid));
		else *slider->value=slider->isLogarithmic ? exp(log(slider->start)+rel*log(slider->end/slider->start)) : slider->start+rel*(slider->end-slider->start);
	}
}

void SliderSet(SLIDER *slider,double *v) {
	if ((*v<slider->start) || (*v>slider->end)) myexitErr("Slider out of range!");
	slider->value=v;
	slider->loc=SliderLoc(slider,*v);
}

double SliderLoc(SLIDER *slider,double v) {
	if (slider->isLogarithmic<0) return allsliders.width*(v<slider->mid ? slider->midfrac*(v-slider->start)/(slider->mid-slider->start) : slider->midfrac+(1.0-slider->midfrac)*log(v/slider->mid)/log(slider->end/slider->mid));
	else return allsliders.width*(slider->isLogarithmic ? log(v/slider->start)/log(slider->end/slider->start) : (v-slider->start)/(slider->end-slider->start));
}

void RenderSliders(SLIDERCONTAINER *sliders) {
	int i,j; XYZ p1,p2; COLOR col={.5,.5,.5},black={0,0,0}; SLIDER *slider; double l; char s[50];
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, camera.screenwidth, -camera.screenheight,0,-1000,1000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(0,0,0,0,0,-1,0,1,0);
	for (i=0;i<sliders->nsliders;i++) {
		slider=&sliders->sliders[i];
		p1.x=sliders->startx; p1.x+=(p1.x<0)*camera.screenwidth+sliders->spacingx*i; p1.y=-sliders->starty; p1.y-=(p1.y>0)*camera.screenheight+sliders->spacingy*i;
		p1.z=-500; p2.x=p1.x+sliders->width; p2.y=p1.y; p2.z=p1.z;
		for (j=0;j<slider->nticks;j++) {
			l=SliderLoc(slider,slider->ticks[j]); glColor3f(0,0,0);
			glBegin(GL_LINES);glVertex3f(p1.x+l,p1.y,p1.z);glVertex3f(p1.x+l,p1.y-8,p1.z);glEnd();
			sprintf(s,"%.3g",slider->ticks[j]);
			DrawString(p1.x+l-2.5*strlen(s),camera.screenheight-(-p1.y+18),s,black,1);
		}
		sprintf(s,"%.3g",*slider->value);
		DrawString(p2.x+10,camera.screenheight-(-p1.y+4),s,black,1);
		MakeCylinder(p1,p2,sliders->height/2,15,col);
		MakeSphere(p1,sliders->endR,col);MakeSphere(p2,sliders->endR,col);
		p1.x+=sliders->sliders[i].loc; MakeSphere(p1,sliders->knobR,col);
	}//need to add labels???
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

}


/*
   Normalize a vector
*/
void Normalize(XYZ *p)
{
   double length;

   length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
   if (length != 0) {
      p->x /= length;
      p->y /= length;
      p->z /= length;
   } else {
      p->x = 0;
      p->y = 0;
      p->z = 0;
   }
}

/*
   Calculate the unit normal at p given two other points
   p1,p2 on the surface. The normal points in the direction
   of p1 crossproduct p2
*/
XYZ CalcNormal(XYZ p,XYZ p1,XYZ p2)
{
   XYZ n,pa,pb;

   pa.x = p1.x - p.x;
   pa.y = p1.y - p.y;
   pa.z = p1.z - p.z;
   pb.x = p2.x - p.x;
   pb.y = p2.y - p.y;
   pb.z = p2.z - p.z;
   Normalize(&pa);
   Normalize(&pb);

   n.x = pa.y * pb.z - pa.z * pb.y;
   n.y = pa.z * pb.x - pa.x * pb.z;
   n.z = pa.x * pb.y - pa.y * pb.x;
   Normalize(&n);

   return(n);
}

void myexit()
{//this doesn't actually exit
	if (writefile) fclose(filepointer);
	free(sphereP); free(drawbig); free(drawbig_inp);
	free(glued);free(push);free(verts);free(edges);free(bonds);free(bondparms);free(weighti);free(parent);
	free(types);free(G_inds);free(G_iv);free(G_ds);free(G_fs);free(G_lengthi);free(G_gluet);free(scratch);
	if (nangles) {free(angles); free(angparams); free(G_angc); free(G_sint); free(G_cost);}
	if (ndihedrals) {free(dihedrals); free(dihparams);}
	if (nimproper) {free(improper); free(impparams);}
	if (VDW) {free(VDW); free(VDW1); free(VDW2);}
	if (p_F) {free(p_F); free(p_F2); free(p_V); free(p_force); free(p_Fimplicit1); free(p_Fimplicit2); free(p_Fvdw); free(p_Fsingle);}
	if (implicitwater) {free(charges);free(atomtype);free(scratchi);}
	if (angmomentum) {free(angloc);free(angvel);free(angfor);free(angvelmax);free(angmomentum);}
	running=0;
	glutLeaveMainLoop();
	glutDestroyWindow(glutGetWindow());
	if (have3DInput) {if (activexinput) mexCallMATLAB(0,NULL,1,&srv,"release"); 
#ifdef __LINUX_INPUT
 	else spnav_close();
#endif
	}
}
void myexitErr(char *err)
{
   if (running) myexit();
   mexErrMsgTxt(err);
}

void do3DInput()
{
	XYZ raw,raw2,amount,X,Y,nZ,rot;
	mxArray *pm,*pm2[2],*pm3;//get(get(get(srv,'Sensor'),'Translation'))
	double s=1*pushscale,sc=.0005*camera.viewheight,sa=.1*pushscale,rotamount;
	if (activexinput) {
		pm2[0]=srv;pm2[1]=mxCreateString("Sensor"); mexCallMATLAB(1,&pm,2,pm2,"get");
		mxDestroyArray(pm2[1]);
		pm2[0]=pm; pm2[1]=mxCreateString("Translation"); mexCallMATLAB(1,&pm,2,pm2,"get");
		mxDestroyArray(pm2[1]);
		pm2[1]=mxCreateString("Rotation"); mexCallMATLAB(1,&pm3,2,pm2,"get");
		mxDestroyArray(pm2[0]); 
		mxDestroyArray(pm2[1]);
		pm2[0]=pm; mexCallMATLAB(1,&pm,1,pm2,"get");
		raw.x=*(double*)mxGetData(mxGetField(pm,0,"X"))*s;
		raw.y=*(double*)mxGetData(mxGetField(pm,0,"Y"))*s;raw.z=*(double*)mxGetData(mxGetField(pm,0,"Z"))*s;
		mxDestroyArray(pm2[0]); mxDestroyArray(pm);
		pm2[0]=pm3; mexCallMATLAB(1,&pm,1,pm2,"get");
		raw2.x=*(double*)mxGetData(mxGetField(pm,0,"X"));raw2.y=*(double*)mxGetData(mxGetField(pm,0,"Y"));raw2.z=*(double*)mxGetData(mxGetField(pm,0,"Z"));
		rotamount=*(double*)mxGetData(mxGetField(pm,0,"Angle"))*sa;
		mxDestroyArray(pm2[0]); mxDestroyArray(pm);
	} else {
#ifdef LINUX_INPUT
		raw.x=0;raw.y=0;raw.z=0;raw2.x=0;raw2.y=0;raw2.z=0;
		spnav_event sev;
		int t;
		while ((t=spnav_poll_event(&sev))!=0)
			if (t==SPNAV_EVENT_MOTION) {
				raw.x=sev.motion.x;raw.y=sev.motion.y;raw.z=sev.motion.z;
				raw2.x=sev.motion.rx;raw2.y=sev.motion.ry;raw2.z=sev.motion.rz;
			}
		double n=sqrt(raw.x*raw.x+raw.y*raw.y+raw.z*raw.z);
		if (n<50) n=50; if (n>350) n=350; n/=79.5; n=n*n*n*s;
		Normalize(&raw);
		raw.x*=n;raw.y*=n;raw.z*=-n;
		n=sqrt(raw2.x*raw2.x+raw2.y*raw2.y+raw2.z*raw2.z);
		if (n<50) n=50; if (n>350) n=350; n/=79.5; n=n*n*n*sa;
		Normalize(&raw2);raw2.z=-raw2.z;
		rotamount=n;
#endif
	}
	nZ=camera.vd; Normalize(&nZ); Y=camera.vu; Normalize(&Y); CROSSPROD(nZ,Y,X);
	amount.x=raw.x*X.x+raw.y*Y.x-raw.z*nZ.x;
	amount.y=raw.x*X.y+raw.y*Y.y-raw.z*nZ.y;
	amount.z=raw.x*X.z+raw.y*Y.z-raw.z*nZ.z;
	rot.x=raw2.x*X.x+raw2.y*Y.x-raw2.z*nZ.x;
	rot.y=raw2.x*X.y+raw2.y*Y.y-raw2.z*nZ.y;
	rot.z=raw2.x*X.z+raw2.y*Y.z-raw2.z*nZ.z;
	if (showcursor) {cursorpos.x+=amount.x*sc;cursorpos.y+=amount.y*sc;cursorpos.z+=amount.z*sc;}
	else doForce(push,npush,amount,rot,rotamount);
}

void doForce(int *vpush,int npush,XYZ amount,XYZ rot,double rotamount)
{
	int i; XYZ t,t2,cm={0,0,0},tv;
	memset(p_force,0,nvert*sizeof(XYZ));
	double moment=0,tw=0,c,w;
	if (npush) {
		for (i=0;i<npush;i++) {tv=verts[vpush[i]];w=1.0/weighti[vpush[i]];CROSSPROD(rot,tv,t); tw+=w;cm.x+=tv.x*w;cm.y+=tv.y*w;cm.z+=tv.z*w;}
		moment+=10; cm.x=cm.x/tw;cm.y=cm.y/tw;cm.z=cm.z/tw;
		for (i=0;i<npush;i++) {tv=verts[vpush[i]];tv.x-=cm.x;tv.y-=cm.y;tv.z-=cm.z;w=1.0/weighti[vpush[i]];CROSSPROD(rot,tv,t); moment+=(t.x*t.x+t.y*t.y+t.z*t.z)*w;}
		for (i=0;i<npush;i++) {tv=verts[vpush[i]]; tv.x-=cm.x;tv.y-=cm.y;tv.z-=cm.z;c=rotamount*tw/moment;CROSSPROD(rot,tv,t);p_force[vpush[i]].x=t.x*c+amount.x/npush;p_force[vpush[i]].y=t.y*c+amount.y/npush;p_force[vpush[i]].z=t.z*c+amount.z/npush;}
		for (i=0;i<nvert;i++) {p_force[i].x-=amount.x/nvert;p_force[i].y-=amount.y/nvert;p_force[i].z-=amount.z/nvert;}
	}
}

void pos2ang(XYZ *verts,XYZ *p_V,XYZ *p_F,XYZ *angloc,XYZ *angvel,XYZ *angfor) {
	double (*mat)[4][3]=(double(*)[4][3])scratch,(*M)[3]; memset(mat,0,12*nvert*sizeof(double));//allocate scratch big enough!!!
	double *lengthi=scratch+(nvert+2)*12,(*diff)[3]=(double (*)[3])(scratch+(nvert+2)*13),
		*cosa=scratch+(nvert+2)*19,*sina=scratch+(nvert+2)*20;
	XYZ *cross=(XYZ *)(scratch+(nvert+2)*16);
	for (int i=0;i<nvert+1;i++) {
		int j=parent[i];diff[i][0]=verts[i].x-verts[j].x,diff[i][1]=verts[i].y-verts[j].y,diff[i][2]=verts[i].z-verts[j].z;
		lengthi[i]=1.0/(angloc[i].x=sqrt(diff[i][0]*diff[i][0]+diff[i][1]*diff[i][1]+diff[i][2]*diff[i][2]));}
	for (int i=0;i<nvert;i++) {
		int j=parent[i],k=parent[j];
		double dx1=verts[i].x-verts[j].x,dy1=verts[i].y-verts[j].y,dz1=verts[i].z-verts[j].z,dx2=verts[j].x-verts[k].x,dy2=verts[j].y-verts[k].y,dz2=verts[j].z-verts[k].z;
		cross[i].x=diff[i][1]*diff[j][2]-diff[i][2]*diff[j][1];cross[i].y=diff[i][2]*diff[j][0]-diff[i][0]*diff[j][2];cross[i].z=diff[i][0]*diff[j][1]-diff[i][1]*diff[j][0];
		double dp=diff[i][0]*diff[j][0]+diff[i][1]*diff[j][1]+diff[i][2]*diff[j][2];
		angloc[i].y=acos(cosa[i]=-dp*lengthi[i]*lengthi[j]); sina[i]=sqrt(1.0-cosa[i]*cosa[i]);
	}
	for (int i=nvert-1;i>0;i--) {
		int j=parent[i],k=parent[j],l=parent[k]; M=mat[i];
		mat[i][0][0]+=verts[i].x*p_F[i].x;mat[i][0][1]+=verts[i].x*p_F[i].y;mat[i][0][2]+=verts[i].x*p_F[i].z;
		mat[i][1][0]+=verts[i].y*p_F[i].x;mat[i][1][1]+=verts[i].y*p_F[i].y;mat[i][1][2]+=verts[i].y*p_F[i].z;
		mat[i][2][0]+=verts[i].z*p_F[i].x;mat[i][2][1]+=verts[i].z*p_F[i].y;mat[i][2][2]+=verts[i].z*p_F[i].z;
		mat[i][3][0]+=p_F[i].x;mat[i][3][1]+=p_F[i].y;mat[i][3][2]+=p_F[i].z;
		XYZ vd1; vd1.x=p_V[i].x-p_V[j].x;vd1.y=p_V[i].y-p_V[j].y;vd1.z=p_V[i].z-p_V[j].z;
		angvel[i].x=(vd1.x*diff[i][0]+vd1.y*diff[i][1]+vd1.z*diff[i][2])*lengthi[i];
		XYZ vd2; vd2.x=p_V[j].x-p_V[k].x;vd2.y=p_V[j].y-p_V[k].y;vd2.z=p_V[j].z-p_V[k].z;
		double c2=lengthi[i]*lengthi[j]/sina[i],c1=c2*cosa[i]*lengthi[i]*angloc[j].x,c3=c2,c4=c3*cosa[i]*lengthi[j]*angloc[i].x;
		angvel[i].y=vd1.x*(c1*diff[i][0]+c2*diff[j][0])+vd2.x*(c3*diff[i][0]+c4*diff[j][0])+vd1.y*(c1*diff[i][1]+c2*diff[j][1])+vd2.y*(c3*diff[i][1]+c4*diff[j][1])+vd1.z*(c1*diff[i][2]+c2*diff[j][2])+vd2.z*(c3*diff[i][2]+c4*diff[j][2]);
		angloc[i].z=-acos(lengthi[i]*lengthi[j]*lengthi[j]*lengthi[k]/(sina[i]*sina[j])*.99999999999999*(cross[i].x*cross[j].x+cross[i].y*cross[j].y+cross[i].z*cross[j].z));
		if (diff[i][0]*cross[j].x+diff[i][1]*cross[j].y+diff[i][2]*cross[j].z<0) angloc[i].z=-angloc[i].z;
		double c0=-lengthi[i]*lengthi[j]/(sina[i]*sina[i]); c1=c0*lengthi[i]; c2=c0*cosa[i]*lengthi[j]; 
		c0=-lengthi[j]*lengthi[k]/(sina[j]*sina[j]);c3=c0*cosa[j]*lengthi[j]; c4=c0*lengthi[k];
		XYZ vd3; vd3.x=p_V[k].x-p_V[l].x;vd3.y=p_V[k].y-p_V[l].y;vd3.z=p_V[k].z-p_V[l].z;
		angvel[i].z=-((c1*vd1.x+c2*vd2.x)*cross[i].x+(c3*vd2.x+c4*vd3.x)*cross[j].x+(c1*vd1.y+c2*vd2.y)*cross[i].y+(c3*vd2.y+c4*vd3.y)*cross[j].y+(c1*vd1.z+c2*vd2.z)*cross[i].z+(c3*vd2.z+c4*vd3.z)*cross[j].z);
		angfor[i].x=lengthi[i]*(M[3][0]*diff[i][0]+M[3][1]*diff[i][1]+M[3][2]*diff[i][2]);
		double cn=lengthi[i]*lengthi[j]/sina[i]; XYZ crossn={cross[i].x*cn,cross[i].y*cn,cross[i].z*cn};
		angfor[i].y=-(crossn.x*(M[2][1]-M[1][2])+crossn.y*(M[0][2]-M[2][0])+crossn.z*(M[1][0]-M[0][1])
			+M[3][0]*(-verts[j].y*crossn.z+verts[j].z*crossn.y)+M[3][1]*(-verts[j].z*crossn.x+verts[j].x*crossn.z)+M[3][2]*(-verts[j].x*crossn.y+verts[j].y*crossn.x));
		angfor[i].z=-lengthi[j]*(diff[j][0]*(M[2][1]-M[1][2])+diff[j][1]*(M[0][2]-M[2][0])+diff[j][2]*(M[1][0]-M[0][1])
			+M[3][0]*(-verts[j].y*diff[j][2]+verts[j].z*diff[j][1])+M[3][1]*(-verts[j].z*diff[j][0]+verts[j].x*diff[j][2])+M[3][2]*(-verts[j].x*diff[j][1]+verts[j].y*diff[j][0]));
		mat[j][0][0]+=mat[i][0][0];mat[j][0][1]+=mat[i][0][1];mat[j][0][2]+=mat[i][0][2];
		mat[j][1][0]+=mat[i][1][0];mat[j][1][1]+=mat[i][1][1];mat[j][1][2]+=mat[i][1][2];
		mat[j][2][0]+=mat[i][2][0];mat[j][2][1]+=mat[i][2][1];mat[j][2][2]+=mat[i][2][2];
		mat[j][3][0]+=mat[i][3][0];mat[j][3][1]+=mat[i][3][1];mat[j][3][2]+=mat[i][3][2];
	}
//	for (int i=0;i<30;i++) printf("%3d: %14g,%14g,%14g;%14g,%14g,%14g;%14g,%14g,%14g\n",i,angloc[i].x,angloc[i].y,angloc[i].z,angvel[i].x,angvel[i].y,angvel[i].z,angfor[i].x,angfor[i].y,angfor[i].z);
//	for (int i=10;i<30;i++) printf("%14g\n",angvel[i].z);
		//signs of everything above are pretty random!!!; need to initialize 2 dummy atoms

		//need to have two parent vertices of 0th atom, with parent and verts arrays expanded accordingly
		//first 3 backbone atoms are glued to varying degrees
	//need an order[] to evaluate stuff, also need some scratch to compile the 3x4 matrices of force coefficients
}//worry about degrees/radians?

void ang2pos(XYZ *verts,XYZ *p_V,XYZ *p_F,XYZ *angloc,XYZ *angvel,XYZ *angfor) {
	//recenter should zero out verts[0] and p_V[0]! and, initialize p_V[0] to 0 here too??
	double *sina=scratch,*cosa=scratch+(nvert+2),*lengthi=scratch+2*(nvert+2);XYZ *diff=(XYZ*)(scratch+3*(nvert+2)),*ddiffn=(XYZ*)(scratch+6*(nvert+2)),*crossn=(XYZ*)(scratch+9*(nvert+2)),*dcrossn=(XYZ*)(scratch+12*(nvert+2));
	sina[0]=1;cosa[0]=0;lengthi[0]=lengthi[nvert]=1;diff[0].x=0;diff[0].y=1;diff[0].z=0;diff[nvert].x=1;diff[nvert].y=0;diff[nvert].z=0;memset(ddiffn,0,sizeof(XYZ));memset(ddiffn+nvert,0,sizeof(XYZ));memset(dcrossn,0,sizeof(XYZ));crossn[0].x=crossn[0].y=0;crossn[0].z=-1;
	memset(angvel,0,sizeof(XYZ));memset(p_V,0,sizeof(XYZ));
	for (int i=1;i<nvert;i++) {
		int j=parent[i],k=parent[j]; lengthi[i]=1.0/angloc[i].x;
		double sia=sin(angloc[i].y),coa=cos(angloc[i].y),sid=-sin(angloc[i].z),cod=cos(angloc[i].z),sii=1.0/sina[j];
		XYZ norm={-diff[k].x*sii*lengthi[k]-diff[j].x*sii*lengthi[j]*cosa[j],-diff[k].y*sii*lengthi[k]-diff[j].y*sii*lengthi[j]*cosa[j],-diff[k].z*sii*lengthi[k]-diff[j].z*sii*lengthi[j]*cosa[j]};
		verts[i].x=verts[j].x+angloc[i].x*(-diff[j].x*lengthi[j]*coa+norm.x*sia*cod+crossn[j].x*sia*sid);
		verts[i].y=verts[j].y+angloc[i].x*(-diff[j].y*lengthi[j]*coa+norm.y*sia*cod+crossn[j].y*sia*sid);
		verts[i].z=verts[j].z+angloc[i].x*(-diff[j].z*lengthi[j]*coa+norm.z*sia*cod+crossn[j].z*sia*sid);
		diff[i].x=verts[i].x-verts[j].x;diff[i].y=verts[i].y-verts[j].y;diff[i].z=verts[i].z-verts[j].z;
		sina[i]=sia;cosa[i]=coa;
		double dsii=-cosa[j]*sii*sii*angvel[j].y;XYZ dnorm={-ddiffn[k].x*sii-diff[k].x*lengthi[k]*dsii-ddiffn[j].x*sii*cosa[j]+diff[j].x*lengthi[j]*sii*sii*angvel[j].y,-ddiffn[k].y*sii-diff[k].y*lengthi[k]*dsii-ddiffn[j].y*sii*cosa[j]+diff[j].y*lengthi[j]*sii*sii*angvel[j].y,-ddiffn[k].z*sii-diff[k].z*lengthi[k]*dsii-ddiffn[j].z*sii*cosa[j]+diff[j].z*lengthi[j]*sii*sii*angvel[j].y};
		crossn[i].x=crossn[j].x*cod-norm.x*sid;crossn[i].y=crossn[j].y*cod-norm.y*sid;crossn[i].z=crossn[j].z*cod-norm.z*sid;
		dcrossn[i].x=dcrossn[j].x*cod-dnorm.x*sid-(norm.x*cod+crossn[j].x*sid)*angvel[i].z;dcrossn[i].y=dcrossn[j].y*cod-dnorm.y*sid-(norm.y*cod+crossn[j].y*sid)*angvel[i].z;dcrossn[i].z=dcrossn[j].z*cod-dnorm.z*sid-(norm.z*cod+crossn[j].z*sid)*angvel[i].z;
		p_V[i].x=p_V[j].x+diff[i].x*lengthi[i]*angvel[i].x+angloc[i].x*(diff[j].x*lengthi[j]*sia*angvel[i].y-ddiffn[j].x*coa+norm.x*(coa*cod*angvel[i].y-sia*sid*angvel[i].z)+dnorm.x*sia*cod+crossn[j].x*(coa*sid*angvel[i].y+sia*cod*angvel[i].z)+dcrossn[j].x*sia*sid);
		p_V[i].y=p_V[j].y+diff[i].y*lengthi[i]*angvel[i].x+angloc[i].x*(diff[j].y*lengthi[j]*sia*angvel[i].y-ddiffn[j].y*coa+norm.y*(coa*cod*angvel[i].y-sia*sid*angvel[i].z)+dnorm.y*sia*cod+crossn[j].y*(coa*sid*angvel[i].y+sia*cod*angvel[i].z)+dcrossn[j].y*sia*sid);
		p_V[i].z=p_V[j].z+diff[i].z*lengthi[i]*angvel[i].x+angloc[i].x*(diff[j].z*lengthi[j]*sia*angvel[i].y-ddiffn[j].z*coa+norm.z*(coa*cod*angvel[i].y-sia*sid*angvel[i].z)+dnorm.z*sia*cod+crossn[j].z*(coa*sid*angvel[i].y+sia*cod*angvel[i].z)+dcrossn[j].z*sia*sid);
		ddiffn[i].x=((p_V[i].x-p_V[j].x)-diff[i].x*lengthi[i]*angvel[i].x)*lengthi[i];
		ddiffn[i].y=((p_V[i].y-p_V[j].y)-diff[i].y*lengthi[i]*angvel[i].x)*lengthi[i];
		ddiffn[i].z=((p_V[i].z-p_V[j].z)-diff[i].z*lengthi[i]*angvel[i].x)*lengthi[i];
	}
}


void matrixvectormult3(double * __restrict mat,XYZ * __restrict vect,XYZ * __restrict ans,const int n,const int m) {
	for (int i=0;i<m;i++) {
		XYZ a={0,0,0};
		for (int j=0;j<n;j++) {a.x+=mat[j]*vect[j].x;a.y+=mat[j]*vect[j].y;a.z+=mat[j]*vect[j].z;}
		mat+=n; ans[i].x=a.x;ans[i].y=a.y;ans[i].z=a.z;
	}
}

template <bool fullfeatures> void doPhysics()
{
	int i,it,e1,e2,a1,a2,e3;
	XYZ f,*p1,*p2,*p3,d,*cr1,*cr2,*ds=G_ds,*fs=G_fs,*angc=G_angc;
	double length,fm,li,dp,li1,li2,li3,c1,c2,c2b,si,si1,si2,co1,co2,ang,fc,temp,temp2,temp3,dp2,dp3,co,cri,dp12,dp23,dp13;
	double *lengthi=G_lengthi,*sint=G_sint,*cost=G_cost,ri;
	int *gluet=G_gluet;
	double E_elect=0,E_vdw=0,E_bond=0,E_angle=0,E_dihed=0,E_dihed_bump=0;
	GGB=0;
	count++; fric=1-parms.fric;//if (count<fricchange) fric=fric1; else fric=1-parms.fric;//fric2;
	if (writefile & (count%writefreq==1)) for (i=0;(i<nvert) && (toupper(types[i][1])!='W' || toupper(types[i][0])=='C');i++) fprintf(filepointer,"%.3f %.3f %.3f\n",verts[i].x,verts[i].y,verts[i].z);//write(verts[i],8,3,filepointer);
	memset(gluet,0,nvert*sizeof(int)); for (i=0;i<nglued;i++) gluet[glued[i]]=1;
//	for (it=0;it<simiters;it++) {
		memcpy(p_F,p_force,nvert*sizeof(XYZ));//note: if you press enter while pulling, the force keeps pulling???
		for (i=0;i<nedge;i++) {
			p1=verts+bonds[i][0]; p2=verts+bonds[i][1];
			ds[i].x=d.x=p1->x-p2->x; ds[i].y=d.y=p1->y-p2->y; ds[i].z=d.z=p1->z-p2->z;
			length=sqrt(d.x*d.x+d.y*d.y+d.z*d.z); lengthi[i]=li=1.0/length;
			fm=(bondparms[i][1]-length)*bondparms[i][0];//we multiplied by 2 when we loaded params
			if (fullfeatures) E_bond+=fm*(bondparms[i][1]-length)*.5;
			fs[i].x=d.x*li*fm; fs[i].y=d.y*li*fm; fs[i].z=d.z*li*fm;
//			p1=p_F+bonds[i][0]; p2=p_F+bonds[i][1];
//			p1->x=p1->x+f.x; p1->y=p1->y+f.y; p1->z=p1->z+f.z;
//			p2->x=p2->x-f.x; p2->y=p2->y-f.y; p2->z=p2->z-f.z;
		}
		for (i=0;i<nangles;i++) {
			e1=angles[i][0]; e2=angles[i][1];
			p1=ds+e1; p2=ds+e2;
			dp=p1->x*p2->x+p1->y*p2->y+p1->z*p2->z;
			li1=lengthi[e1]; li2=lengthi[e2];
			c1=li1*li2; si=c1*dp; cost[i]=si; ang=acos(si); sint[i]=si=sqrt(1-si*si); 
			fc=(ang-angparams[i][1])*angparams[i][0]; if (fullfeatures) E_angle+=fc*(ang-angparams[i][1])*.5;
			c1*=fc/si; c2b=c1*dp; c2=c2b*li2*li2;// /si???
			fs[e2].x+=p1->x*c1-p2->x*c2; fs[e2].y+=p1->y*c1-p2->y*c2; fs[e2].z+=p1->z*c1-p2->z*c2;
			c2=c2b*li1*li1;
			fs[e1].x-=p1->x*c2-p2->x*c1; fs[e1].y-=p1->y*c2-p2->y*c1; fs[e1].z-=p1->z*c2-p2->z*c1;
			angc[i].x=p1->y*p2->z-p1->z*p2->y;angc[i].y=p1->z*p2->x-p1->x*p2->z;angc[i].z=p1->x*p2->y-p1->y*p2->x;
		}
		for (i=0;i<ndihedrals;i++) {
			a1=dihedrals[i][0]; a2=dihedrals[i][1];
			e1=dihedrals[i][2]; e2=dihedrals[i][3]; e3=dihedrals[i][4];
			cr1=angc+a1; cr2=angc+a2;
			dp=cr1->x*cr2->x+cr1->y*cr2->y+cr1->z*cr2->z;
			li1=lengthi[e1]; li2=lengthi[e2]; li3=lengthi[e3];
			si1=sint[a1];si2=sint[a2];co1=cost[a1];co2=cost[a2];
			ang=acos(dp*li1*li2*li2*li3/(si1*si2)*.99999999999999);
			p1=ds+e1;
			if (p1->x*cr2->x+p1->y*cr2->y+p1->z*cr2->z<0) ang=-ang;
			fc=0;
			if (!fullfeatures) {
				if ((temp=dihparams[i][2])<-50) {
					double x=ang-dihparams[i][1]; x+=TWOPI*(x<-PI);
					double g=exp(-x*x*dihparams[i][0])*(temp+100);
					fc-=g*2*x*dihparams[i][0];
					i++;
				}
				while ((temp=dihparams[i][2])<0) {
					fc+=dihparams[i][0]*sin(ang*temp+dihparams[i][1]);
					i++;
				}
				fc+=dihparams[i][0]*sin(ang*dihparams[i][2]-dihparams[i][1]);
			} else {
				if ((temp=dihparams[i][2])<-50) {
					double x=ang-dihparams[i][1]; x+=TWOPI*(x<-PI);
					double g=exp(-x*x*dihparams[i][0])*(temp+100);
					E_dihed_bump+=g; fc-=g*2*x*dihparams[i][0];
					i++;
				}
				while ((temp=dihparams[i][2])<0) {
					fc+=dihparams[i][0]*sin(ang*temp+dihparams[i][1]);
					E_dihed-=dihparams[i][0]*(cos(ang*temp+dihparams[i][1]))/temp;
					i++;
				}
				fc+=dihparams[i][0]*sin(ang*dihparams[i][2]-dihparams[i][1]);
				E_dihed-=dihparams[i][0]*(cos(ang*dihparams[i][2]-dihparams[i][1]))/dihparams[i][2];
			}
			temp=-fc*li1*li2/(si1*si1); temp2=temp*li1;
			fs[e1].x+=temp2*cr1->x;fs[e1].y+=temp2*cr1->y;fs[e1].z+=temp2*cr1->z;
			temp2=-temp*co1*li2; temp=-dihedrals[i][5]*fc*li2*li3/(si2*si2); temp3=-temp*co2*li2;
			fs[e2].x+=temp2*cr1->x+temp3*cr2->x;fs[e2].y+=temp2*cr1->y+temp3*cr2->y;fs[e2].z+=temp2*cr1->z+temp3*cr2->z;
			temp2=temp*li3;
			fs[e3].x+=temp2*cr2->x;fs[e3].y+=temp2*cr2->y;fs[e3].z+=temp2*cr2->z;
		}
		for (i=0;i<nedge;i++) {
			e1=bonds[i][0]; e2=bonds[i][1];
			p_F[e1].x+=fs[i].x;p_F[e1].y+=fs[i].y;p_F[e1].z+=fs[i].z;
			p_F[e2].x-=fs[i].x;p_F[e2].y-=fs[i].y;p_F[e2].z-=fs[i].z;
		}
		memset(p_Fvdw,0,nvert*sizeof(XYZ)); memset(p_Fsingle,0,nvert*sizeof(XYZ));
		for (i=0;i<nVDW;i++) {
			double ri2,ri3,ri6,ri8,ri12,ri14;
			e1=VDW1[i]; e2=VDW2[i];
			p1=verts+e1; p2=verts+e2;
			d.x=p2->x-p1->x; d.y=p2->y-p1->y; d.z=p2->z-p1->z;
			ri=1/sqrt(d.x*d.x+d.y*d.y+d.z*d.z);
			if (!fullfeatures) {
				ri2=ri*ri; ri3=ri2*ri; ri6=ri3*ri3; ri8=ri6*ri2; ri14=ri8*ri6;
				fc=VDW[i].x*ri14-VDW[i].y*ri8+VDW[i].z*ri3;
			} else {
				ri2=ri*ri; ri3=ri2*ri; ri6=ri3*ri3; ri12=ri6*ri6;
				double t1=VDW[i].x*ri12*(1.0/12),t2=VDW[i].y*ri6*(1.0/6),t3=VDW[i].z*ri;
				fc=ri2*(t1*12-t2*6+t3); E_elect+=t3; E_vdw+=t1-t2;
				if ((e1==singleatomforces) || (e2==singleatomforces)) {
					p_Fsingle[e1].x-=d.x*fc;p_Fsingle[e1].y-=d.y*fc;p_Fsingle[e1].z-=d.z*fc;
					p_Fsingle[e2].x+=d.x*fc;p_Fsingle[e2].y+=d.y*fc;p_Fsingle[e2].z+=d.z*fc;
				}
			}
//			if (fc>10000) fc=10000; if (fc<-10000) fc=-10000;
			d.x*=fc; d.y*=fc; d.z*=fc; 
			p_Fvdw[e1].x-=d.x; p_Fvdw[e1].y-=d.y; p_Fvdw[e1].z-=d.z;
			p_Fvdw[e2].x+=d.x; p_Fvdw[e2].y+=d.y; p_Fvdw[e2].z+=d.z;
		}
		for (i=0;i<nvert;i++) {p_F[i].x+=p_Fvdw[i].x;p_F[i].y+=p_Fvdw[i].y;p_F[i].z+=p_Fvdw[i].z;}
		if (implicitwater) {//malloc and free arrays: I,reff,G_I,G_R,R_I
//$$$			const double scale[]={1.091,.484,.7,1.066,.8};//last one for S is just made up
			const double scale[]={0.85,0.72,0.79,0.85,0.96};
			int j,t1,t2; double p1x,p1y,p1z,p2x,p2y,p2z,r1,sc1,r2,sc2,dx,dy,dz,d,di,dsq,myforce,re1,re2;
			double d01,m01,d02,m02,r1s,r2s,L,U,Li,Ui,Ui2mLi2,myexp,myexp2,myinv,mytanh,mysech,gi1,gi2,temp4,tempsc;
			double r,ra,psi,psi2,arg;
			double *I=(double*)scratchi,*reff=(double*)scratchi+nvert,*G_I=(double*)scratchi+2*nvert,*G_R=(double*)scratchi+3*nvert,*R_I=(double*)scratchi+4*nvert;
//$$$			const double alpha=1.095,beta=1.908,gamma=2.508,Sneck=.362/4/PI;
			const double alpha=1,beta=.8,gamma=4.85,Sneck=0;
			double radius[5]={1.2,1.7,1.55,1.52,1.8},tablem0[5][5]={{0.353,0.454,0.426,0.421,0.472},
			{0.143,0.19,0.177,0.174,0.199},{0.183,0.241,0.225,0.222,0.252},{0.193,0.254,0.237,0.233,0.265},
			{0.122,0.164,0.152,0.15,0.171}},tablet0[5][5]={{2.68,3.153,3.008,2.98,3.25},
			{3.244,3.7,3.559,3.531,3.795},{3.075,3.536,3.395,3.367,3.631},
			{3.041,3.503,3.362,3.333,3.599},{3.355,3.809,3.669,3.642,3.903}};
			memset(I,0,nvert*sizeof(double));memset(G_R,0,nvert*sizeof(double));
			memset(p_Fimplicit1,0,nvert*sizeof(XYZ));memset(p_Fimplicit2,0,nvert*sizeof(XYZ));
			for (i=0;i<5;i++) for (j=0;j<5;j++) tablem0[i][j]*=Sneck;
			for (i=0;i<nvert;i++) {
				p1x=verts[i].x;p1y=verts[i].y;p1z=verts[i].z;t1=atomtype[i];r1=radius[t1]-.09;//$$$-.09
				sc1=scale[t1];
				for (j=i+1;j<nvert;j++) {
					p2x=verts[j].x;p2y=verts[j].y;p2z=verts[j].z;t2=atomtype[j];r2=radius[t2]-.09;//$$$-.09
					sc2=scale[t2];
					dx=p2x-p1x;dy=p2y-p1y;dz=p2z-p1z;
					d=sqrt(dx*dx+dy*dy+dz*dz); di=1.0/d;
/*$$$					d01=tablet0[t1][t2];m01=tablem0[t1][t2];d02=tablet0[t2][t1];m02=tablem0[t2][t1];//table lookup from Mongan, Simmerling et al.
					temp=(d-d01);temp*=temp; I[i]+=m01/(1.0+temp+.3*temp*temp*temp);//if (i==0) printf("%12g; ",I[i]);
					temp=(d-d02);temp*=temp; I[j]+=m02/(1.0+temp+.3*temp*temp*temp);*/
					r2s=r2*sc2; r1s=r1*sc1;
					if (d+r2s>=r1) {//first compute I_i
						L=(d-r2s>r1)?d-r2s:r1; U=d+r2s;Li=1.0/L;Ui=1.0/U;Ui2mLi2=Ui*Ui-Li*Li;
						I[i]+=.5*(Li-Ui+.25*d*Ui2mLi2+.5*di*log(L*Ui)-.25*r2s*r2s*di*Ui2mLi2);
					}//if (i==0) printf("%12g\n",I[i]);
					if (d+r1s>=r2) {//compute I_j
						L=(d-r1s>r2)?d-r1s:r2; U=d+r1s;Li=1.0/L;Ui=1.0/U;Ui2mLi2=Ui*Ui-Li*Li;
						I[j]+=.5*(Li-Ui+.25*d*Ui2mLi2+.5*di*log(L*Ui)-.25*r1s*r1s*di*Ui2mLi2);
					}
				}
			} //output(I,nvert);
			for (i=0;i<nvert;i++) {
				t1=atomtype[i];r=radius[t1];ri=1.0/r;ra=r-.09;psi=ra*I[i];psi2=psi*psi;arg=alpha*psi-beta*psi2+gamma*psi*psi2;
				if (arg>40) {mytanh=1; mysech=0;} else {myexp=exp(arg); myexp2=myexp*myexp; myinv=1.0/(myexp2+1);mytanh=(myexp2-1)*myinv; mysech=myexp*2*myinv;}
				reff[i]=1.0/(1.0/ra-ri*mytanh); R_I[i]=ra*(alpha-beta*psi*2+gamma*psi2*3)*mysech*mysech*ri*reff[i]*reff[i];//printf("%12g\n",reff[i]);
			}//printf("R0: %.12g, R_I0: %.12g, I0: %.12g, R1: %.12g, R_I1: %.12g, I1: %.12g\n",reff[0],R_I[0],I[0],reff[1],R_I[1],I[1]);
			for (i=0;i<nvert;i++) {
				c1=charges[i];re1=reff[i];p1x=verts[i].x;p1y=verts[i].y;p1z=verts[i].z;
				for (j=i+1;j<nvert;j++) {
					c2=charges[j];re2=reff[j];p2x=verts[j].x;p2y=verts[j].y;p2z=verts[j].z;
					dx=p2x-p1x;dy=p2y-p1y;dz=p2z-p1z; dsq=dx*dx+dy*dy+dz*dz;
					myexp=exp(-dsq/(4*re1*re2)); temp=1.0/sqrt(dsq+re1*re2*myexp);
					tempsc=temp*(.9873*332)*c1*c2; GGB-=tempsc; temp2=temp*temp*tempsc;  temp3=(.5+.125*dsq/(re1*re2))*myexp;
					G_R[i]+=temp3*temp2*re2; G_R[j]+=temp3*temp2*re1;
					myforce=(1.0-.25*myexp)*temp2;
					p_Fimplicit1[i].x+=dx*myforce;p_Fimplicit1[j].x-=dx*myforce;
					p_Fimplicit1[i].y+=dy*myforce;p_Fimplicit1[j].y-=dy*myforce;
					p_Fimplicit1[i].z+=dz*myforce;p_Fimplicit1[j].z-=dz*myforce;//check signs of these forces!!!
				}
			}//printf("R0: %.12g, R1: %.12g, G_R0: %.12g, G_R1: %.12g\n",reff[0],reff[1],G_R[0],G_R[1]);
			//printf("%.12g\n",reff[0]);
			for (i=0;i<nvert;i++) {c1=charges[i]; ri=1.0/reff[i]; temp=(.9873*332*.5)*ri*c1*c1; GGB-=temp; G_R[i]+=temp*ri;}
			for (i=0;i<nvert;i++) G_I[i]=G_R[i]*R_I[i];
			//printf("G: %.12g, G_I0: %.12g, I0: %.12g, G_I1: %.12g, I1: %.12g\n",GGB,G_I[0],I[0],G_I[1],I[1]);
			for (i=0;i<nvert;i++) {
				p1x=verts[i].x;p1y=verts[i].y;p1z=verts[i].z;t1=atomtype[i];r1=radius[t1]-.09;//$$$-.09
				sc1=scale[t1];gi1=G_I[i];
				for (j=i+1;j<nvert;j++) {
					p2x=verts[j].x;p2y=verts[j].y;p2z=verts[j].z;t2=atomtype[j];r2=radius[t2]-.09;//$$$-.09
					sc2=scale[t2];gi2=G_I[j];
					dx=p2x-p1x;dy=p2y-p1y;dz=p2z-p1z;
					dsq=dx*dx+dy*dy+dz*dz; d=sqrt(dsq); //di=1.0/d;
/*$$$					d01=tablet0[t1][t2];m01=tablem0[t1][t2];d02=tablet0[t2][t1];m02=tablem0[t2][t1];//table lookup from Mongan, Simmerling et al.
					temp=(d-d01);temp2=temp*temp;temp3=temp2*temp2*temp;temp4=1.0+temp2+.3*temp3*temp;myforce=-gi1*(2.0*temp+1.8*temp3)*m01/(temp4*temp4*d);
					temp=(d-d02);temp2=temp*temp;temp3=temp2*temp2*temp;temp4=1.0+temp2+.3*temp3*temp;myforce-=gi2*(2.0*temp+1.8*temp3)*m02/(temp4*temp4*d);*/
					r2s=r2*sc2; r1s=r1*sc1;
					if (d+r2s>=r1) {//first compute I_i
						L=(d-r2s>r1)?d-r2s:r1; U=d+r2s; temp=L*L*U*U;
						myforce=-gi1*((U-L)*(U+L)*(dsq+r2s*r2s)+2.0*temp*log(L/U))/(temp*8*d*dsq);//$$$changed -= to =-
					}
					if (d+r1s>=r2) {//compute I_j
						L=(d-r1s>r2)?d-r1s:r2; U=d+r1s; temp=L*L*U*U;
						myforce-=gi2*((U-L)*(U+L)*(dsq+r1s*r1s)+2.0*temp*log(L/U))/(temp*8*d*dsq);
					}
					p_Fimplicit2[i].x+=dx*myforce;p_Fimplicit2[j].x-=dx*myforce;
					p_Fimplicit2[i].y+=dy*myforce;p_Fimplicit2[j].y-=dy*myforce;
					p_Fimplicit2[i].z+=dz*myforce;p_Fimplicit2[j].z-=dz*myforce;//check signs of these forces!!!
				}
			}//slightly uncomfortable about the force field ignoring/downscaling connected atoms, but doing implicit water calculations for *all* pairs; side effect that local similarly charged atoms *attract*, for example the three hydrogens in the ammonia group of NASN
			//there's noise online that '99 parameters don't work well with implicit water, and bias things towards helices
			for (i=0;i<nvert;i++) {p_F[i].x+=p_Fimplicit1[i].x+p_Fimplicit2[i].x;p_F[i].y+=p_Fimplicit1[i].y+p_Fimplicit2[i].y;p_F[i].z+=p_Fimplicit1[i].z+p_Fimplicit2[i].z;}
		}
		memcpy(p_F2,p_F,nvert*sizeof(XYZ));
		if (FORCE_M) matrixvectormult3(FORCE_M,p_F2,p_F,nvert,nvert); else if (bondmomentum) {
//memset(p_F,0,nvert*sizeof(XYZ));p_F[5].x=1;
			int j;memset(scratch,0,nvert*sizeof(double));double e=bondmomentum,r;for(i=nvert-1;i>0;i--) {j=parent[i];r=e*(scratch[i]=1.0/(scratch[i]+e+1));scratch[j]+=e*(1.0-r);p_F[j].x+=r*p_F[i].x;p_F[j].y+=r*p_F[i].y;p_F[j].z+=r*p_F[i].z;}
			r=1.0/(scratch[0]+1);p_F[0].x*=r;p_F[0].y*=r;p_F[0].z*=r;for(i=1;i<nvert;i++) {j=parent[i];r=scratch[i];p_F[i].x=r*(p_F[i].x+e*p_F[j].x);p_F[i].y=r*(p_F[i].y+e*p_F[j].y);p_F[i].z=r*(p_F[i].z+e*p_F[j].z);}
		}
/*		memset(p_F,0,nvert*sizeof(XYZ));p_F[104].x=1;
		double m=.95;int j;for (i=nvert-1;i>=1;i--) {j=parent[i];p_F[j].x+=m*p_F[i].x;p_F[j].y+=m*p_F[i].y;p_F[j].z+=m*p_F[i].z;}
		for (i=1;i<nvert;i++) {j=parent[i];p_F[i].x+=m*p_F[j].x;p_F[i].y+=m*p_F[j].y;p_F[i].z+=m*p_F[j].z;}*/
		if (angmomentum) {//memset(p_F,0,nvert*sizeof(XYZ));p_F[20].x=1;
			pos2ang(verts,p_V,p_F,angloc,angvel,angfor);
			for (i=1;i<nvert;i++) {//make sure first few atoms stay fixed as they're supposed to!!!
				angvel[i].x=angvel[i].x*fric+angfor[i].x*angmomentum[i].x; angvel[i].y=angvel[i].y*fric+angfor[i].y*angmomentum[i].y;angvel[i].z=angvel[i].z*fric+angfor[i].z*angmomentum[i].z;
				if (angvel[i].x>angvelmax[i].x) {angvel[i].x=angvelmax[i].x; nvfix++;}if (angvel[i].x<-angvelmax[i].x) {angvel[i].x=-angvelmax[i].x; nvfix++;}
				if (angvel[i].y>angvelmax[i].y) {angvel[i].y=angvelmax[i].y; nvfix++;}if (angvel[i].y<-angvelmax[i].y) {angvel[i].y=-angvelmax[i].y; nvfix++;}
				if (angvel[i].z>angvelmax[i].z) {angvel[i].z=angvelmax[i].z; nvfix++;}if (angvel[i].z<-angvelmax[i].z) {angvel[i].z=-angvelmax[i].z; nvfix++;}
				if (angmomentum[i].x==0) angvel[i].x=0;if (angmomentum[i].y==0) angvel[i].y=0;if (angmomentum[i].z==0) angvel[i].z=0;
				angloc[i].x+=angvel[i].x;angloc[i].y+=angvel[i].y;angloc[i].z+=angvel[i].z;
			}
//	for (int i=0;i<15;i++) printf("%3d: %14g,%14g,%14g;\n",i,angvel[i].x,angvel[i].y,angvel[i].z);
			ang2pos(verts,p_V,p_F,angloc,angvel,angfor);
		} else {
			for (i=0;i<nvert;i++) {
				if (!gluet[i]) {
					p_V[i].x=p_V[i].x*fric+p_F[i].x*weighti[i]; p_V[i].y=p_V[i].y*fric+p_F[i].y*weighti[i]; p_V[i].z=p_V[i].z*fric+p_F[i].z*weighti[i];
					double v=p_V[i].x*p_V[i].x+p_V[i].y*p_V[i].y+p_V[i].z*p_V[i].z;
					if (v>C_MAX_V_SQRD) {double vfix=sqrt(C_MAX_V_SQRD/v); p_V[i].x*=vfix;p_V[i].y*=vfix;p_V[i].z*=vfix; nvfix++;}
				}	verts[i].x+=p_V[i].x; verts[i].y+=p_V[i].y; verts[i].z+=p_V[i].z;
//###			}// else {p_V[i].x=p_V[i].y=p_V[i].z=0;}//????
			}
		}//part of recenter must be to recenter molecules inside the box! also, need to sync this with water cycles???
//		memcpy(p_F,p_F2,nvert*sizeof(XYZ));
		if (outputcount!=-1) {
			Energies[outputcount+noutput]=GGB;Energies[outputcount+noutput*2]=E_elect;
			Energies[outputcount+noutput*3]=E_vdw;Energies[outputcount+noutput*4]=E_bond;
			Energies[outputcount+noutput*5]=E_angle;Energies[outputcount+noutput*6]=E_dihed;
			Energies[outputcount+noutput*7]=E_dihed_bump;
			double ke=0;
			for (i=0;i<nvert;i++) {
				out_P[nvert*3*outputcount+i]=verts[i].x;out_P[nvert*(1+3*outputcount)+i]=verts[i].y;out_P[nvert*(2+3*outputcount)+i]=verts[i].z;
				out_V[nvert*3*outputcount+i]=p_V[i].x;out_V[nvert*(1+3*outputcount)+i]=p_V[i].y;out_V[nvert*(2+3*outputcount)+i]=p_V[i].z;
				out_F[nvert*3*outputcount+i]=p_F[i].x;out_F[nvert*(1+3*outputcount)+i]=p_F[i].y;out_F[nvert*(2+3*outputcount)+i]=p_F[i].z;
				if (!gluet[i]) ke+=.5*(p_V[i].x*p_V[i].x+p_V[i].y*p_V[i].y+p_V[i].z*p_V[i].z)/weighti[i];
			}
			Energies[outputcount]=ke;
			outputcount++; if (outputcount==noutput) myexit();
		}
		if (exitAfterPhysics) myexit();
//	}
}

mxArray* eigenSetup(double delta) {
	XYZ *f,*p;
	double *t;
	int i;
	mxArray* out;
	f=(XYZ*)malloc(nvert*sizeof(XYZ));
	p=(XYZ*)malloc(nvert*sizeof(XYZ));
	memset(p_force,0,nvert*sizeof(XYZ));
	memcpy(p,verts,nvert*sizeof(XYZ));
	nglued=0; //interesting variations without this;
	doPhysics<0>();
	memcpy(f,p_F,nvert*sizeof(XYZ));
	t=(double*)mxGetData(out=mxCreateDoubleMatrix(3*nvert,3*nvert,mxREAL));
	for (i=0;i<nvert;i++) {
		eigensub(&verts[i].x,f,p,t+3*nvert*3*i,delta);
		eigensub(&verts[i].y,f,p,t+3*nvert*(3*i+1),delta);
		eigensub(&verts[i].z,f,p,t+3*nvert*(3*i+2),delta);
	} 	memcpy(verts,p,nvert*sizeof(XYZ));
	free(f);free(p);
	return out;
}//in principle might want to copy back velocities at the end

void eigensub(double *change,XYZ *f,XYZ *p,double *out,double delta) {
	int i;
	memcpy(verts,p,nvert*sizeof(XYZ)); *change+=delta;
	doPhysics<0>();
	for (i=0;i<nvert;i++) {out[3*i]=(p_F[i].x-f[i].x)/delta;out[3*i+1]=(p_F[i].y-f[i].y)/delta;out[3*i+2]=(p_F[i].z-f[i].z)/delta;}
}

void framerate() {
	int time; COLOR black={0,0,0};
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"FPS:%4.2f; physics count:%d",
			frame*1000.0/(time-timebase),count);
		timebase = time;		
		frame = 0;
	} 
	DrawString(20,20,s,black,0);
}

void DrawString(int x,int y,char *s,COLOR col,int which) {
	char *c; 
	glColor3f(col.r,col.g,col.b);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, camera.screenwidth, 0.0, camera.screenheight);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);

  // set position to start drawing fonts
	glRasterPos2f(x,y);
  // loop all the characters in the string
	for (c=s; *c != '\0'; c++) {
		glutBitmapCharacter(which?GLUT_BITMAP_TIMES_ROMAN_10:GLUT_BITMAP_8_BY_13, *c);
	}
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
