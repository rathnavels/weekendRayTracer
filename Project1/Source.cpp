
#include <cstdlib>
#include <iostream>
#include <GL\freeglut.h>
#include <stdlib.h>
#include <omp.h>
#include <sstream>
#include <fstream>
#include <string>
#include <glm\glm.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "Plane.h"
#include "float.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Camera.h"
#include "time.h"
#include "Triangle.h"

using namespace std;
using namespace glm;

#define _MSC_VER

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define nSamples 15
#define FOV 90
#define DEPTH 20
#define width 400
#define height 400



unsigned char *pixmap;
vec3 normal_at_hitPoint;
hitable_list *world;
Camera *cam;
float tmin, tmax;
clock_t t1, t2;
int diff;
int seconds;
float RUNTIME_SECONDS;



void save_screenshot(string filename, int w, int h)
{
	
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	int nSize = w*h * 3;
	char* dataBuffer = (char*)malloc(nSize*sizeof(char));

	if (!dataBuffer) return;

	
	glReadPixels((GLint)0, (GLint)0,
		(GLint)w, (GLint)h,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, dataBuffer);

	int timest = (unsigned)time(NULL);
	string ts,ts1;         
	ostringstream convert, convert1;  
	convert << timest;     
	ts = convert.str();
	seconds = (int)RUNTIME_SECONDS;
	convert1 << RUNTIME_SECONDS;
	ts1 = convert1.str();
	filename = filename + "_" + ts + "_" + ts1 + "s" + ".tga";
	FILE *filePtr = fopen(filename.c_str(), "wb");
	if (!filePtr) return;


	unsigned char TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char header[6] = { w % 256, w / 256,
		h % 256, h / 256,
		24, 0 };
	// We write the headers
	fwrite(TGAheader, sizeof(unsigned char), 12, filePtr);
	fwrite(header, sizeof(unsigned char), 6, filePtr);
	// And finally our image data
	fwrite(dataBuffer, sizeof(GLubyte), nSize, filePtr);
	fclose(filePtr);

	free(dataBuffer);

	
}



vec3 colorFunc(Ray ray, hitable_list *world, int depth)
{
	hitRecord  rec;
	if (world->hit(ray, 0.001, FLT_MAX, rec))
	{
		Ray scattered;
		vec3 attenuation;
		vec3 target = rec.hitPoint + rec.normal + random_in_unitSphere();
		if (depth < DEPTH && rec.mat_ptr->scatter(ray, rec, attenuation, scattered))
		{
			return attenuation * colorFunc(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
		return vec3(0.85, 0.85, 0.85);
}

void setPixels()
{
	
	t1 = clock();
	Ray ray;
	vec3 color;
	//vec3 origin = vec3(0, 0, 0);
	//vec3 lowerLeft = vec3(-1.6, -0.9, -1.0);
	//vec3 vertical = vec3(0, 1.8, 0);
	//vec3 horizontal = vec3(3.2, 0, 0);

	
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int i = (y * width + x) * 3;
				vec3 temp = vec3(0., 0., 0.);
				for (int k = 0; k < nSamples; k++)
				{
					float v = (float)(y+ (float)(rand()) / RAND_MAX) / (float)height;
					float u = (float)(x + (float)(rand()) / RAND_MAX) / (float)width;
				
				Ray r=cam->getRay(u, v);
				color += colorFunc(r,world,0);
			}
				color /= (float)nSamples;
				color = vec3(sqrt(color.x), sqrt(color.y), sqrt(color.z));
				pixmap[i++] = (int)(255.99 * color.x);
				pixmap[i++] = (int)(255.99 * color.y);
				pixmap[i] = (int)(255.99 * color.z);
		}
	}
		t2 = clock();
		diff = (float)t2 - (float)t1;
		cout << "Time taken : " << diff / CLOCKS_PER_SEC << endl;
		RUNTIME_SECONDS = diff / CLOCKS_PER_SEC;
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
static void processKey(unsigned char button, int x, int y)
{
	if (button == 's')
	{
		save_screenshot("Image", width, height);
	}
}
static void init(void)
{
	glClearColor(1, 1, 1, 1); // Set background color.
}

int main(int argc, char *argv[])
{
	pixmap = new unsigned char[width * height * 3];
	vec3 lookFrom, lookAt, cameraUp;
	lookFrom = vec3(0, 0, 0);
	lookAt = vec3(0, 0, -1);
	cameraUp = vec3(0, 1, 0);
	float flen = (lookFrom - lookAt).length();
	cam = new Camera(FOV, (float)width / height, lookFrom, lookAt, cameraUp, flen, 100);
	hitable *list[4];
	list[0] = new Sphere(vec3(0, 0, -1.5), 0.3, new Lambertian(vec3(0.8, 0.2, 0.2)));
	list[2] = new Sphere(vec3(-.6, 0, -1.5), 0.3, new GlossyMetal(vec3(0.2, 0.8, 0.2)));
	list[3] = new Sphere(vec3(0.6, 0, -1.5), 0.3, new Metal(vec3(0.2, 0.2, 0.8)));
	list[1] = new Plane(vec3(0, -0.3, -1), vec3(0, 1, 0), new Lambertian(vec3(0.7, 0.7, 0.7)));
	//world = new hitable_list(list, 4);

	vec3 A, B, C, D, E, F, G, H;
	A = vec3(-0.6, -0.6, -0.8);
	B = vec3(0.6, -0.6, -0.8);
	C = vec3(-0.6, 0.6, -0.8);
	D = vec3(0.6, 0.6, -0.8);
	E = vec3(-0.6, -0.6, -1.6);
	F = vec3(0.6, -0.6, -1.6);
	G = vec3(-0.6, 0.6, -1.6);
	H = vec3(0.6, 0.6, -1.6);

	hitable *cornelBox[10];
	//Bottom
	cornelBox[0] = new Triangle(E, A, B, new Lambertian(vec3(0.9, 0.9, 0.9)));
	cornelBox[1] = new Triangle(E, B, F, new Lambertian(vec3(0.9, 0.9, 0.9)));

	//Top
	cornelBox[2] = new Triangle(C, G, D, new Lambertian(vec3(0.9, 0.9, 0.9)));
	cornelBox[3] = new Triangle(G, H, D, new Lambertian(vec3(0.9, 0.9, 0.9)));

	//Left
	cornelBox[4] = new Triangle(A, C, G, new Lambertian(vec3(0.8, 0.2, 0.2)));
	cornelBox[5] = new Triangle(A, G, E, new Lambertian(vec3(0.8, 0.2, 0.2)));

	//Right
	cornelBox[6] = new Triangle(B, D, H, new Lambertian(vec3(0.2, 0.2, 0.8)));
	cornelBox[7] = new Triangle(H, F, B, new Lambertian(vec3(0.2, 0.2, 0.8)));

	//Back
	cornelBox[8] = new Triangle(G, E, F, new Lambertian(vec3(0.9, 0.9, 0.9)));
	cornelBox[9] = new Triangle(F, H, G, new Lambertian(vec3(0.9, 0.9, 0.9)));

	world = new hitable_list(cornelBox, 10);

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
	glutKeyboardFunc(processKey);
	glutMainLoop();

	return 0;
}
