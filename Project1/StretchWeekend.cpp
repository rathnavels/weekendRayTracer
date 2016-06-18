
#include <cstdlib>
#include <iostream>
#include <GL\freeglut.h>
#include <stdlib.h>
#include <omp.h>
#include <glm\glm.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "float.h"




using namespace std;
using namespace glm;


int width, height;
unsigned char *pixmap;
vec3 normal_at_hitPoint;


float tmin, tmax;






void setPixels()
{
	Ray ray;
	vec3 color=vec3(0.5,0.5,0.5);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int i = (y * width + x) * 3;
			pixmap[i++] = (int)(255.99 * color.x);
			pixmap[i++] = (int)(255.99 * color.y);
			pixmap[i] = (int)(255.99 * color.z);
		}
	}
}

static void windowResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w / 2, 0, h / 2, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
static void windowDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);
	glFlush();
}
static void processMouse(int button, int state, int x, int y)
{
	if (state == GLUT_UP){}
	//	exit(0);               // Exit on mouse click.
}
static void init(void)
{
	glClearColor(1, 1, 1, 1); // Set background color.
}

int main(int argc, char *argv[])
{

	width = 1280;
	height = 720;



	setPixels();

	//GL
	glutInit(&argc, argv);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutCreateWindow("Rathinavel - Ray Tracing");
	init();
	glutReshapeFunc(windowResize);
	glutDisplayFunc(windowDisplay);
	glutMouseFunc(processMouse);
	glutMainLoop();

	return 0;
}
