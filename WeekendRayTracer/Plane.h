#ifndef PLANE_H
#define PLANE_H


#include "Hitable.h"
#include "glm\glm.hpp"
class Plane: public hitable
{
public:
	Plane(){}
	Plane(vec3 point, vec3 normal, Material* mp): Point(point), Normal(normal), mat_ptr(mp){}
	virtual bool hit(Ray ray, float t_min, float t_max, hitRecord& record);
	vec3 Point, Normal;
	Material* mat_ptr;

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
		record.matPtr = mat_ptr;
		return true;
	}
	return false;
}

#endif