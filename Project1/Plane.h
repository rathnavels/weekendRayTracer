#ifndef PLANE_H
#define PLANE_H



#include "glm\glm.hpp"
#include "Hitable.h"
class Plane : public hitable
{
public:
	Plane(){}
	Plane(vec3 point, vec3 normal, Material *mptr) : Point(point), Normal(normal), mat_ptr(mptr){};
	virtual bool hit(Ray ray, float t_min, float t_max, hitRecord& record);
	vec3 Point, Normal;
	Material *mat_ptr;

};


bool Plane::hit(Ray ray, float t_min, float t_max, hitRecord& record)
{
	float temp;
	float AdotN = dot(Point, Normal);
	float OdotN = dot(ray.O, Normal);
	float DdotN = dot(ray.D, Normal);
	temp = (float)(AdotN - OdotN) / DdotN;
	if (temp<t_max && temp> t_min)
	{
		record.t = temp;
		record.hitPoint = ray.point_at_parameter(record.t);
		record.normal = Normal;
		record.mat_ptr = this->mat_ptr;
		return true;
	}
	return false;
}

#endif