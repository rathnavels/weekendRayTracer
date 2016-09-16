
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

#define nSamples 100
#define FOV 90
#define DEPTH 50
#define width 700
#define height 700



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
	string ts,ts1,ts2;         
	ostringstream convert, convert1, convert2;  
	convert << timest;     
	ts = convert.str();
	seconds = (int)RUNTIME_SECONDS;
	convert1 << RUNTIME_SECONDS;
	ts1 = convert1.str();
	int samples = nSamples;
	convert2 << samples;
	ts2 = convert2.str();
	filename = filename + "_" + ts + "_" + ts1 + "s" + "_" + ts2 + "samples" + ".tga";
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
		vec3 emitted = rec.mat_ptr->emitted(0, 0, rec.hitPoint);
		if (depth < DEPTH && rec.mat_ptr->scatter(ray, rec, attenuation, scattered))
		{
			return emitted + attenuation * colorFunc(scattered, world, depth + 1);
		}
		else
		{
			return emitted;
		}
	}
	else
		return vec3(0,0,0);
}

void setPixels()
{
	
	t1 = clock();
	Ray ray;
	vec3 color;


	
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

	//Setup Camera
	vec3 lookFrom, lookAt, cameraUp;
	lookFrom = vec3(0, 0, 0);
	lookAt = vec3(0, 0, -1);
	cameraUp = vec3(0, 1, 0);
	float flen = (lookFrom - lookAt).length();
	cam = new Camera(FOV, (float)width / height, lookFrom, lookAt, cameraUp, flen, 100);

	//Setup Scene
	hitable *list[5];
	Texture *constant1 = new constant_texture(vec3(0.9, 0.9, 0.9));
	Texture *constant5 = new constant_texture(vec3(4,4,4));
	Texture *constant2 = new constant_texture(vec3(0.6, 0.2, 0.6));
	Texture *constant3 = new constant_texture(vec3(0.35, 0.1, 0.1));
	Texture *constant4 = new constant_texture(vec3(0.1, 0.35, 0.1));
	Texture *checker1 = new checker_texture(constant1, constant2, 7);
	Texture *checker2 = new checker_texture(constant4, constant3, 55);
	//list[0] = new Sphere(vec3(0, 0, -1.5), 0.3, new Lambertian(vec3(0.8, 0.2, 0.2)));
	list[0] = new Sphere(vec3(0, 0, -1.5), 0.3, new Dielectric(1.3));
	//list[2] = new Sphere(vec3(-.6, 0, -1.5), 0.3, new GlossyMetal(vec3(0.2, 0.8, 0.2)));
	list[2] = new Sphere(vec3(-.6, 0, -1.5), 0.3, new diffuse_light(constant1));
	//list[3] = new Sphere(vec3(0.6, 0, -1.5), 0.3, new Metal(vec3(0.2, 0.2, 0.8)));
	list[3] = new Sphere(vec3(0.6, 0, -1.5), 0.3, new diffuse_light(constant1));
	list[4] = new Sphere(vec3(0., -0.2, -0.9), 0.1, new Lambertian(checker2));
	//list[4] = new Sphere(vec3(0., -0.2, -0.9), 0.1, new diffuse_light(constant1));
	list[1] = new Plane(vec3(0, -0.3, -1), vec3(0, 1, 0), new Lambertian(checker1));
	//list[4] = new Sphere(vec3(0, 0, -0.8), 0.2, new Dielectric(1.5));
	
	//world = new hitable_list(list, 5);

	vec3 A, B, C, D, E, F, G, H, Ci, Gi, Di, Hi;
	A = vec3(-0.6, -0.6, -0.8);
	B = vec3(0.6, -0.6, -0.8);
	C = vec3(-0.6, 0.6, -0.8);
	D = vec3(0.6, 0.6, -0.8);
	E = vec3(-0.6, -0.6, -1.6);
	F = vec3(0.6, -0.6, -1.6);
	G = vec3(-0.6, 0.6, -1.6);
	H = vec3(0.6, 0.6, -1.6);

	Ci = vec3(-0.3, 0.6, -1.1);
	Di = vec3(0.3, 0.6, -1.1);
	Gi = vec3(-0.3, 0.6, -1.3);
	Hi = vec3(0.3, 0.6, -1.3);

	hitable *cornelBox[12];
	//Bottom
	cornelBox[0] = new Triangle(E, A, B, new Lambertian(new constant_texture(vec3(0.9, 0.9, 0.9))));
	cornelBox[1] = new Triangle(E, B, F, new Lambertian(new constant_texture(vec3(0.9, 0.9, 0.9))));

	//Top
	cornelBox[2] = new Triangle(C, G, D, new Lambertian(new constant_texture(vec3(0.9, 0.9, 0.9))));
	cornelBox[3] = new Triangle(G, H, D, new Lambertian(new constant_texture(vec3(0.9, 0.9, 0.9))));

	cornelBox[10] = new Triangle(Ci, Gi, Di, new diffuse_light(constant1));
	cornelBox[11] = new Triangle(Gi, Hi, Di, new diffuse_light(constant1));

	//Left
	cornelBox[4] = new Triangle(A, C, G, new Lambertian(new constant_texture(vec3(0.8, 0.2, 0.2))));
	cornelBox[5] = new Triangle(A, G, E, new Lambertian(new constant_texture(vec3(0.8, 0.2, 0.2))));

	//Right
	cornelBox[6] = new Triangle(B, D, H, new Lambertian(new constant_texture(vec3(0.2, 0.2, 0.8))));
	cornelBox[7] = new Triangle(H, F, B, new Lambertian(new constant_texture(vec3(0.2, 0.2, 0.8))));

	//Back
	cornelBox[8] = new Triangle(G, E, F, new Lambertian(new constant_texture(vec3(0.9, 0.9, 0.9))));
	cornelBox[9] = new Triangle(F, H, G, new Lambertian(new constant_texture(vec3(0.9, 0.9, 0.9))));

	world = new hitable_list(cornelBox, 12);

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
