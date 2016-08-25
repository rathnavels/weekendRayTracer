#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include "glm\glm.hpp"
#include "Hitable.h"


using namespace glm;

class Triangle : public hitable
{
public:

	Triangle(){}
	Triangle(vec3 A, vec3 B, vec3 C, Material *mptr) : aa(A), bb(B), cc(C), mat_ptr(mptr){};
	virtual bool hit(Ray ray, float t_min, float t_max, hitRecord& rec);
	vec3 aa, bb, cc;
	Material *mat_ptr;
};

bool Triangle::hit(Ray ray, float t_min, float t_max, hitRecord& rec)
{
	vec3 side1 = normalize(aa - cc);
	vec3 side2 = normalize(bb - cc);
	vec3 Normal = cross(side1, side2);
	/*float D = dot(Normal, a);
	float temp = (dot(Normal, ray.O) + D) / dot(Normal, ray.D);

	if (temp <t_max && temp > t_min)
	{
	rec.t = temp;
	rec.hitPoint = ray.point_at_parameter(rec.t);
	rec.normal = Normal;
	rec.mat_ptr = this->mat_ptr;
	return true;
	}*/

	float a, b, c, d, e, f, g, h, i, j, k, l;
	a = aa.x - bb.x;
	b = aa.y - bb.y;
	c = aa.z - bb.z;
	d = aa.x - cc.x;
	e = aa.y - cc.y;
	f = aa.z - cc.z;
	g = ray.D.x;
	h = ray.D.y;
	i = ray.D.z;
	j = aa.x - ray.O.x;
	k = aa.y - ray.O.y;
	l = aa.z - ray.O.z;
	float M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);

	float beta = j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g);
	beta = beta / M;

	if (beta < 0.0 || beta >= 1.0)
		return false;

	float gamma = i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c);
	gamma = gamma / M;

	if (gamma < 0.0 || beta + gamma >= 1.0)
		return false;

	float temp = f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c);
	temp = temp / (-M);


	if (temp<t_max && temp> t_min)
	{
		//std::cout << beta + gamma << " " << beta << " " << gamma << "\n";
		rec.t = temp;
		rec.hitPoint = ray.point_at_parameter(rec.t);
		rec.normal = Normal;
		rec.mat_ptr = this->mat_ptr;
		return true;
	}
	return false;
}


#endif