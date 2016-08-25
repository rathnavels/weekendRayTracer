#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "glm\glm.hpp"
using namespace std;

#define PI 3.1415


vec3 random_in_unitDisk()
{
	vec3 p;
	do
	{
		p = (2.0f * vec3(((float)(rand()) / (RAND_MAX + 1)), ((float)(rand()) / (RAND_MAX + 1)), 0)) - vec3(1.0, 1.0, 0.0);
	} while (dot(p, p) >= 1);
	return p;
}

class Camera
{
public:
	Camera(float fov, float aspectRatio, vec3 lookFrom, vec3 lookAt, vec3 vup, float focalLength, float aperture)
	{
		vec3 u, v, w;
		float theta = fov * PI / 180;
		float halfHeight = tan(theta / 2.);
		float halfWidth = aspectRatio * halfHeight;
		lensRadius = aperture / 2;


		origin = lookFrom;
		w = normalize(lookFrom - lookAt);
		u = normalize(cross(vup, w));
		v = normalize(cross(w, u));
		/*vertical = vec3(0.0, 2 * halfHeight, 0.0);
		horizontal = vec3(2 * halfWidth, 0.0, 0.0);
		lower_left_corner = vec3(-halfWidth, -halfHeight, -1.0);*/
		lower_left_corner = origin - halfWidth*u*focalLength - halfHeight*v*focalLength - w*focalLength;
		horizontal = 2 * halfWidth * u * focalLength;
		vertical = 2 * halfHeight * v * focalLength;
	}
	Ray getRay(float s, float t)
	{
		vec3 rd = lensRadius* random_in_unitDisk();
		vec3 offset = u * rd.x + v * rd.y;
		return Ray(origin, lower_left_corner + (s* horizontal) + (t * vertical) - origin - offset);
	}
	vec3 origin, lower_left_corner, vertical, horizontal;
	vec3 u, v, w;
	float lensRadius;
};
















#endif