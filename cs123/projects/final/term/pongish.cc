/**
 * Brett Foster
 * 0239553
 **/
/* Derived from scene.c in the The OpenGL Programming Guide */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "gltree.hh"
#include <vector>


#define LIGHT_0_X 1000
#define LIGHT_0_Y 1000
#define LIGHT_0_Z 200
#define LIGHT_0_W 0

#define LIGHT_1_X -1000
#define LIGHT_1_Y 0
#define LIGHT_1_Z 200
#define LIGHT_1_W 0

static const GLfloat light_position_1[] = { LIGHT_0_X, LIGHT_0_Y, LIGHT_0_Z, LIGHT_0_W };
static const GLfloat light_position_2[] = { LIGHT_1_X, LIGHT_1_Y, LIGHT_1_Z, LIGHT_1_W };
#if 0
static const GLfloat light_colour_b_am[]  = { 0.05, 0.0, 0.0, 1.0 };
static const GLfloat light_colour_b_sp[]  = { 0.1, 0.00, 0.00, 1.0 };
static const GLfloat light_colour_a_am[]  = { 0.05, 0.0, 0.0, 1.0 };
static const GLfloat light_colour_a_sp[]  = { 0.1, 0.00, 0.00, 1.0 };
#else
static const GLfloat light_colour_a_am[]  = { 0.01, 0.01, 0.01, 1.0 };
static const GLfloat light_colour_a_sp[]  = { 0.001, 0.001, 0.001, 1.0 };
static const GLfloat light_colour_b_am[]  = { 0.2, 0.2, 0.2, 1.0 };
static const GLfloat light_colour_b_sp[]  = { 0.01, 0.01, 0.01, 1.0 };
#endif
static const GLfloat ground[] = {0.0, 0.9, 0.5, 1.0};


struct tree_entry {
	int index;
	float x,y,z,rotate;
};

std::vector<GLtree *> trees;
std::vector<struct tree_entry> coords;


/*  Initialize material property and light source.
 */
void init (void) {

	/* if lighting is turned on then use ambient, diffuse and specular
	lights, otherwise use ambient lighting only */
	glLightfv (GL_LIGHT0, GL_AMBIENT, light_colour_a_am);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, light_colour_a_sp);
	glLightfv (GL_LIGHT0, GL_SPECULAR, light_colour_a_sp);
	glLightfv (GL_LIGHT0, GL_POSITION, light_position_1);

	glLightfv (GL_LIGHT1, GL_AMBIENT, light_colour_b_am);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, light_colour_b_sp);
	glLightfv (GL_LIGHT1, GL_SPECULAR, light_colour_b_sp);
	glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);

	glEnable (GL_LIGHTING);
	//glEnable (GL_LIGHT0);
	glEnable (GL_LIGHT1);
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

}




void display (void)
{
	glClearColor(0,0,0,0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* draw surfaces as either smooth or flat shaded */
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix ();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE | GL_SPECULAR, ground);
	
	//tree->draw(0);
	
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glVertex3f(-1000,1000,0);
	glVertex3f(1000,1000,0);
	glVertex3f(1000,-1000,0);
	glVertex3f(-1000,-1000,0);
	glEnd();
	
	for (unsigned int i = 0; i < coords.size(); i ++) {
		glPushMatrix();
		glTranslatef(coords[i].x,coords[i].y,coords[i].z);
		glRotatef(coords[i].rotate, 0, 0, 1);
		trees[coords[i].index]->draw(0);
		glPopMatrix();
	}
	
	glPopMatrix ();
#ifdef DOUBLE_BUFFER
	glutSwapBuffers();
#else
	glFlush();
#endif
}

double l_angle = -75;
double l_angle_vel = 1;
static float ratio;
void modcoord () {
	

}
//void keyboard(unsigned char key, int x, int y)
//void mouse (int x, int y) {
void reshape (int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	ratio = 1.0* w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	
	// Set the correct perspective.
	gluPerspective(90,ratio,0.1,1000);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glTranslatef (0, 0, -1);
	glScalef(0.001,0.001,0.001);
	glRotatef(l_angle,1,0,0);
	
	glutPostRedisplay();
	 
}
void idle () {
	display();
}

int main(int argc, char** argv) {
	//tree = new GLtree();
	
	//tree->load(stdin);
	
	for (;;) {
		GLtree * tree = new GLtree();
		try { 
			tree->load(stdin);
		} catch (const char * error) {
			delete tree;
			break;
		}
		trees.push_back(tree);
	}
	
	struct tree_entry entry;
	
	for (int i = 1; i < argc; i++) {
		if (sscanf (argv[i], "%d,%f,%f,%f,%f", &entry.index, &entry.x, &entry.y, &entry.z, &entry.rotate) == 5) {
			coords.push_back(entry);
		} else {
			fprintf (stderr, "Invalid Argument[%d]: %s\n",i, argv[i]);
		}
	}

	glutInit(&argc, argv);
#ifdef DOUBLE_BUFFER
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA  | GLUT_DEPTH);
#else
	glutInitDisplayMode (GLUT_RGBA  | GLUT_DEPTH);
#endif
	glutInitWindowSize (1024, 768);

	glutCreateWindow( "TREE" );

	glutDisplayFunc(display);
	glutReshapeFunc( reshape );  
	glutIdleFunc (idle);
	init();
	
	
	
	glutMainLoop();
	return 0; 
}

