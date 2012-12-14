#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "circle.hh"
#include "gltree.hh"


GLtree::GLtree	() {
	memset (this->dna, 0, sizeof(struct tdna) * TDNA_END);
}

GLtree::~GLtree () {
	for (int i = 0; i < TDNA_END; i ++) {
		tdna_free(&this->dna[i]);
	}
}

void GLtree::load (FILE * properties) {
	for (int i = 0; i < TDNA_END; i ++) {
		if (!tdna_load(&this->dna[i], properties)) {
			throw "Unable to load all properties.";
		}
	}
}
		
void GLtree::draw (int depth) {
	if (depth == 0) {
		for (int i = 0; i < TDNA_ANIMATION; i ++) {
			tdna_reset(&this->dna[i], 0);
		}
		float scaleFact = tdna_next_i(&this->dna[DEFAULT_SCALE]);
		glScalef(scaleFact,scaleFact,scaleFact);
	}
	
	int branch_arounds =  tdna_next_i(&this->dna[BRANCHING_AROUND]);
	int branch_segments = tdna_next_i(&this->dna[BRANCHING_SEGMENTS]);
	int helical = tdna_next_i(&this->dna[HELICAL_TWIST_LENGTH]);
	glPushMatrix ();
	
	for (int i = 0; i <= helical; i ++) {
		float radius = tdna_next(&this->dna[STEM_RADIUS]); 
		int detail = tdna_next_i(&this->dna[STEM_DETAIL]);
		int mode = tdna_next_i(&this->dna[DRAW_MODE]);
		
		float color [4] = {0,0,0,1};
		
		color [0] = tdna_next(&this->dna[COLOR_RED]);
		color [1] = tdna_next(&this->dna[COLOR_GREEN]);
		color [2] = tdna_next(&this->dna[COLOR_BLUE]);
		
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE | GL_SPECULAR, color);
		
		if (mode == 1) {
			drawCircle(detail,0,radius,radius);
		} else {
			glBegin(GL_LINES);
			glVertex3f (0,0,0);
			glVertex3f (0,0,1);
			glEnd();
		}
			
		if (helical != i)
			transform_helical();
	}
	
	if ( depth < branch_segments) {
		
			transform_stem();
			draw(depth+1);
		
		for (int i = 0; i < branch_arounds; i ++) {
			glPushMatrix ();
				transform_branch();
				draw(depth+1);
			glPopMatrix ();
			transform_branch_multi();
		}
	}
	glPopMatrix ();
}


 
void GLtree::transform_stem () {
	float br_ratio = tdna_next(&this->dna[STEM_STEM_RATIO]); 
	glTranslatef(0,0,1);
	glScalef(br_ratio,br_ratio,br_ratio);
}
void GLtree::transform_branch () {
	float br_ratio = tdna_next(&this->dna[BRANCH_STEM_RATIO]); 
	float br_angle = tdna_next(&this->dna[BRANCHING_ANGLE_V]);
	float br_animation = tdna_next(&this->dna[ANIMATION_BRANCHING_ANGLE_V]);
	glRotatef(br_angle+br_animation,1,0,0);
	glScalef(br_ratio,br_ratio,br_ratio);
}
void GLtree::transform_branch_multi () {
	float br_angle = tdna_next(&this->dna[BRANCHING_ANGLE_H]);
	float br_animation = tdna_next(&this->dna[ANIMATION_BRANCHING_ANGLE_H]);
	glRotatef(br_angle+br_animation,0,0,1);

}
void GLtree::transform_helical () {
	transform_stem();
	float br_animation_hx = tdna_next(&this->dna[ANIMATION_HELICAL_TWIST_X]);
	float br_animation_hy = tdna_next(&this->dna[ANIMATION_HELICAL_TWIST_Y]);
	glRotatef(tdna_next(&this->dna[HELICAL_TWIST_X])+br_animation_hx,1,0,0);
	glRotatef(tdna_next(&this->dna[HELICAL_TWIST_Y])+br_animation_hy,0,1,0);
}

