
#include <cstdlib>
#include <iostream>
#include <GL\freeglut.h>
#include <stdlib.h>
#include <omp.h>
#include <glm\glm.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "Camera.h"
#include "Plane.h"



using namespace std;
using namespace glm;


int width, height;
unsigned char *pixmap;
vec3 normal_at_hitPoint;
hitable_list *world;
Camera Cam1;

float tmin, tmax;



vec3 Color(Ray ray, hitable_list *world, int depth)
{
	hitRecord record;
	if (world->hit(ray, 0.001, FLT_MAX, record))
	{
		Ray scattered;
		vec3 attenuation;
		float m = record.t;
		if (depth < 10 && record.matPtr->scatter(ray, record, attenuation, scattered))
		{
			return attenuation * Color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = normalize(ray.direction());
		float t = 0.5 * (unit_direction.y + 1.0);
		//return ((float)(1.0 - t) * vec3(0.8, 0.8, 0.8)) + ((float)t * vec3(0.8, 0.8, 0.8));
		return vec3(0.8, 0.8, 0.8);
	}
}


void setPixels()
{
	int ns = 8;
	Ray ray;
	vec3 color;
 	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int i = (y * width + x) * 3;
			for (int k = 0; k < ns; k++)
			{
				float yincr = (float)(y + (float)(rand())/RAND_MAX+1) / (float)height;
				float xincr = (float)(x + (float)(rand())/RAND_MAX+1) / (float)width; 
				ray = Cam1.getRay(xincr, yincr);
				color += Color(ray, world, 0);
			}
			color /= (float)ns;
			color = vec3(sqrt(color.x), sqrt(color.y), sqrt(color.z));
			pixmap[i++] =(int)(255.99 * color.x);
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
	glOrtho(0, w/2, 0, h/2 , 0, 1);
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
	pixmap = new unsigned char[width * height * 3]; 
	hitable *list[4];
	//list[0] = new Sphere(vec3(1.6, -0.5, -4), 0.8, new DiElectric(1.5));
	list[1] = new Sphere(vec3(-1.6, -0.5, -4), 0.8, new Metal(vec3(0.1, 0.1, 0.7)));
	list[3] = new Sphere(vec3(0, -0.5, -4), 0.8, new Lambertian(vec3(0.8, 0.1, 0.1))); 
	list[0] = new Sphere(vec3(1.6, -0.5, -4), 0.8, new Glossy(vec3(0.1, 0.7, 0.1),0.5));
	list[2] = new Plane(vec3(0, -1.3, 0), vec3(0, 1, 0), new Lambertian(vec3(0.7,0.7,0.7)));
	
//	list[4] = new Plane(vec3(-7.0, -1.3, 0), vec3(1, 0, 0), new Lambertian(vec3(0.1, 0.1, 0.85)));
//	list[5] = new Plane(vec3(0.0, -1.3, -7), vec3(0, 0, 1), new Lambertian(vec3(0.85, 0.85, 0.85)));
	world = new hitable_list(list, 4);
	

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
