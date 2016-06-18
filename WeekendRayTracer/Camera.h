#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "glm\glm.hpp"
using namespace std;

class Camera
{
	public:
		Camera()
		{
			origin = vec3(0, 0, 0);
			vertical = vec3(0.0, 2.25, 0.0);
			horizontal = vec3(4.0, 0.0, 0.0);
			lower_left_corner = vec3(-2.0, -1.125, -1.0);
		}
		Ray getRay(float u, float v)
		{
			return Ray(origin, lower_left_corner + (u * horizontal) + (v * vertical) - origin);
		}
		vec3 origin, lower_left_corner, vertical, horizontal;

};


#endif