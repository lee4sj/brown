#include <math.h>
#include <string.h>
#include "mex.h"
#define DEG2RAD 0.017453292519943297

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	int i,j,k;
	if (nrhs!=1) mexErrMsgTxt("Needs one argument");
	if (mxGetClassID(prhs[0])!=mxDOUBLE_CLASS) mexErrMsgTxt("Input must be real");
	const mwSize *size=mxGetDimensions(prhs[0]);
	int nvert=size[0];
	if (size[1]!=6) mexErrMsgTxt("Input must have 6 columns");
	if (nvert<4) mexErrMsgTxt("Input must have 3 dummy rows and at least one data row");
	double *pI[6],*pO[3],*ans;
	pI[0]=(double*)mxGetData(prhs[0]);
	pO[0]=(double*)malloc(nvert*3*sizeof(double));
	ans=(double*)mxGetData(plhs[0]=mxCreateDoubleMatrix(nvert-3,3,mxREAL));
	for (i=1;i<3;i++) pO[i]=pO[0]+i*nvert;
	for (i=1;i<6;i++) pI[i]=pI[0]+i*nvert;
	pO[0][0]=pO[1][0]=pO[2][0]=0;
	pO[0][1]=pI[3][1];pO[1][1]=pO[2][1]=0;
	pO[0][2]=pO[0][1]-pI[3][2]*cos(pI[4][2]*DEG2RAD);
	pO[1][2]=pI[3][2]*sin(pI[4][2]*DEG2RAD);
	pO[2][2]=0;
	for (i=0;i<=2;i++) for (j=0;j<2;j++) pO[j][i]-=pO[j][2];
	int *nincoming=(int*)malloc(nvert*sizeof(int)),*ready=(int*)malloc(nvert*sizeof(int)),nready=0;
	int *sorted=(int*)malloc((nvert-3)*sizeof(int)),ns=0;
	memset(nincoming,0,nvert*sizeof(int));
	for (i=3;i<nvert;i++) {nincoming[(int)pI[0][i]-1]++;nincoming[(int)pI[1][i]-1]++;nincoming[(int)pI[2][i]-1]++;}
	for (i=3;i<nvert;i++) if (nincoming[i]==0) ready[nready++]=i;
	while (nready>0) {
		i=ready[--nready]; sorted[ns++]=i;
		for (j=0;j<3;j++) {k=(int)pI[j][i]-1; if (k>=3) {nincoming[k]--; if (nincoming[k]==0) ready[nready++]=k;}}
	}
	if (ns!=nvert-3) mexErrMsgTxt("Dependency graph has a cycle!");

//	for (i=3;i<nvert;i++) {
	for (j=nvert-4;j>=0;j--) {i=sorted[j];
		int v1=(int)pI[2][i]-1,v2=(int)pI[1][i]-1,v3=(int)pI[0][i]-1;
		double vb[3];
		vb[0]=pO[0][v2]-pO[0][v3];vb[1]=pO[1][v2]-pO[1][v3];vb[2]=pO[2][v2]-pO[2][v3];
		double f=1.0/sqrt(vb[0]*vb[0]+vb[1]*vb[1]+vb[2]*vb[2]);
		vb[0]*=f;vb[1]*=f;vb[2]*=f;
		double v90[3],v0[3],temp[3];
		temp[0]=pO[0][v1]-pO[0][v2];temp[1]=pO[1][v1]-pO[1][v2];temp[2]=pO[2][v1]-pO[2][v2];
		v90[0]=vb[2]*temp[1]-vb[1]*temp[2];v90[1]=vb[0]*temp[2]-vb[2]*temp[0];v90[2]=vb[1]*temp[0]-vb[0]*temp[1];
		f=1.0/sqrt(v90[0]*v90[0]+v90[1]*v90[1]+v90[2]*v90[2]);
		v90[0]*=f;v90[1]*=f;v90[2]*=f;
		v0[0]=v90[2]*vb[1]-v90[1]*vb[2];v0[1]=v90[0]*vb[2]-v90[2]*vb[0];v0[2]=v90[1]*vb[0]-v90[0]*vb[1];
		double a=pI[3][i]*cos(DEG2RAD*pI[4][i]);
		double b=pI[3][i]*sin(DEG2RAD*pI[4][i])*cos(DEG2RAD*pI[5][i]);
		double c=pI[3][i]*sin(DEG2RAD*pI[4][i])*sin(DEG2RAD*pI[5][i]);
		pO[0][i]=pO[0][v3]+a*vb[0]+b*v0[0]+c*v90[0];
		pO[1][i]=pO[1][v3]+a*vb[1]+b*v0[1]+c*v90[1];
		pO[2][i]=pO[2][v3]+a*vb[2]+b*v0[2]+c*v90[2];
	}
	memcpy(ans,pO[0]+3,(nvert-3)*sizeof(double));
	memcpy(ans+(nvert-3),pO[1]+3,(nvert-3)*sizeof(double));
	memcpy(ans+2*(nvert-3),pO[2]+3,(nvert-3)*sizeof(double));
//	double a=pO[0][3],b=pO[1][3],c=pO[2][3];
//	for(i=0;i<nvert-3;i++) {ans[i]-=a;ans[nvert-3+i]-=b;ans[2*(nvert-3)+i]-=c;}
	free(pO[0]);free(sorted);free(nincoming);free(ready);
}