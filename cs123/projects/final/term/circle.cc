#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "circle.hh"
#include <math.h>

void drawCircle (int segments, float degs, float b_rad, float f_rad) {
	GLfloat segDegs = 2*M_PI / segments;
/*
	glBegin(GL_LINES);
	glVertex3f (-1,0,0);
	glVertex3f (1,0,0);
	glVertex3f (0,-1,0);
	glVertex3f (0,1,0);
	glEnd();
	*/
#if 0
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= segments; i ++) {
		glVertex3f (cos(i*segDegs),sin(i*segDegs),0);
	}
	glEnd();
	
	glTranslatef (0,0,1);
	glRotatef (degs, x,y,z);
	
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= segments; i ++) {
		glVertex3f (cos(i*segDegs),sin(i*segDegs),0);
	}
	glEnd();
#endif
#if 1
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= segments; i ++) {
		float x = cos(i*segDegs);
		float y = sin(i*segDegs);
		float normalizing = sqrt(pow(x*b_rad,2)+ pow(y*b_rad,2));
	
		glNormal3f ((x*b_rad)/normalizing,(y*b_rad)/normalizing,0);
		glVertex3f (x*b_rad,y*b_rad,0);
		//glVertex3f ((x*cos(degs)),(y),(x*sin(degs)+1));
		
		float tx = x*cos(degs);
		float ty = y;
		float tz = -x*sin(degs)+1;
		normalizing = sqrt(pow(tx*f_rad,2) + pow(ty*f_rad,2) + pow(tz,2));
		
		//glNormal3f ((tx*f_rad)/normalizing,(ty*f_rad)/normalizing,tz/normalizing);
		glVertex3f (tx*f_rad,ty*f_rad,tz);
		
		//glVertex3f ((x*cos(degs)),(y),(x*sin(degs)+1));
	}
	glEnd();
#endif
}
 
