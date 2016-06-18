#ifndef HITABLE_H
#define HITABLE_H

#include "Ray.h"


struct hitRecord
{
	float t;
	vec3 hitPoint;
	vec3 normal;

};

class hitable
{
public:
	virtual bool hit(Ray ray, float t_min, float t_max, hitRecord& rec) = 0;
};


#endif
