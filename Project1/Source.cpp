
#include <cstdlib>
#include <iostream>
#include <GL\freeglut.h>
#include <stdlib.h>
#include <omp.h>
#include <glm\glm.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "float.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Camera.h"

using namespace std;
using namespace glm;

int width, height;
unsigned char *pixmap;
vec3 normal_at_hitPoint;
hitable_list *world;
Camera cam;
float tmin, tmax;


vec3 random_in_unit_sphere()
{
	vec3 p;
//	do
//	{
		p = 2.0f * vec3((float)(rand() / (RAND_MAX + 1)), (float)(rand() / (RAND_MAX + 1)), (float)(rand() / (RAND_MAX + 1))) - vec3(1.0, 1.0, 1.0);
//	} while (p.length() >= 1);
	return p;
}

vec3 colorFunc(Ray ray, hitable_list *world)
{
	hitRecord  rec;
	if (world->hit(ray, 0.001, FLT_MAX, rec))
	{
		vec3 target = rec.hitPoint + rec.normal + random_in_unit_sphere();
		return 0.5f * colorFunc(Ray(rec.hitPoint, target-rec.hitPoint),world);
	}
	else
		return vec3(0, 0, 0);
}

void setPixels()
{
	Ray ray;
	vec3 color = vec3(.0f, .5f, .5f);
	vec3 origin = vec3(0, 0, 0);
	vec3 lowerLeft = vec3(-1.6, -0.9, -1.0);
	vec3 vertical = vec3(0, 1.8, 0);
	vec3 horizontal = vec3(3.2, 0, 0);
	int nSamples = 8;
	
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int i = (y * width + x) * 3;
				vec3 temp = vec3(0., 0., 0.);
				for (int k = 0; k < nSamples; k++)
				{
					float v = (float)(y+ (float)(rand() / (RAND_MAX + 1))) / float(height);
					float u = (float)(x + (float)(rand() / (RAND_MAX + 1))) / float(width);

				Ray r=cam.getRay(u, v);
				color = colorFunc(r,world);
				temp.x += (int)(255.99 * color.x);
				temp.y += (int)(255.99 * color.y);
				temp.z += (int)(255.99 * color.z);
			}
				pixmap[i++] = (float)temp.x / nSamples;
				pixmap[i++] = (float)temp.y / nSamples;
				pixmap[i] = (float)temp.z / nSamples;
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

	width = 960;
	height = 540;
	pixmap = new unsigned char[width * height * 3];

	hitable *list[2];
	list[0] = new Sphere(vec3(0, 0, -1), 0.3);
	list[1] = new Plane(vec3(0, -0.5, -1), vec3(0,1,0));
	world = new hitable_list(list, 2);

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
