#ifndef SPHERE_H
#define SPHERE_H


#include "glm\glm.hpp"
#include "Hitable.h"

using namespace glm;



enum Refl_t {DIFF, SPEC, REFR};



class Sphere  : public hitable
{
public:
	Sphere(){  }
	Sphere(vec3 cen, float r, Material *mptr) :center(cen), radius(r), mat_ptr(mptr){};
	virtual bool hit(Ray ray, float t_min, float t_max, hitRecord& rec);
	vec3 center;
	float radius;
	vec3 emission, color;
	Refl_t refl_t;
	Material *mat_ptr;


};


bool Sphere::hit(Ray ray, float t_min, float t_max, hitRecord& rec)
{
	float A = dot(ray.D, ray.D);
	float B = 2 * dot(ray.D, (ray.O - center));
	float C = dot(ray.O - center, ray.O - center) - (radius*radius);

	

	float rootPart = (B*B) - (4 * A*C);
	if (rootPart > 0)
	{
		float temp = (-B - sqrt(rootPart)) / (2 * A);
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.hitPoint = ray.point_at_parameter(rec.t);
			rec.normal = (rec.hitPoint - center) / radius;
			rec.mat_ptr = this->mat_ptr;
			return true;
		}
		temp = (-B + sqrt(rootPart)) / (2 * A);
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.hitPoint = ray.point_at_parameter(rec.t);
			rec.normal = (rec.hitPoint - center) / radius;
			rec.mat_ptr = this->mat_ptr;
			return true;
		}
	}
	return false;
}


#endif