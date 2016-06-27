#ifndef HITABLE_H
#define HITABLE_H

#include "Ray.h"

class Material;

struct hitRecord
{
	float t;
	vec3 hitPoint;
	vec3 normal;
	Material *mat_ptr;

};

class hitable
{
public:
	virtual bool hit(Ray ray, float t_min, float t_max, hitRecord& rec) = 0;
};


class Material
{
public:
	virtual bool scatter(Ray& r, hitRecord &rec, vec3& attenuation, Ray& scattered) = 0;
};


vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n)*n;
}

vec3 random_in_unitSphere()
{
	vec3 p;
	p = (2.0f * vec3(((float)(rand()) / (RAND_MAX + 1)), ((float)(rand()) / (RAND_MAX + 1)), ((float)(rand()) / (RAND_MAX + 1)))) - vec3(1.0, 1.0, 1.0);
	return p;
}

class Lambertian : public Material
{
public:
	Lambertian(vec3& a) : albedo(a) {}
	virtual bool scatter(Ray& r_in, hitRecord& rec, vec3& attenuation, Ray& scattered)
	{
		vec3 target = rec.hitPoint + rec.normal + random_in_unitSphere();
		scattered = Ray(rec.hitPoint, target-rec.hitPoint);
		attenuation = albedo;
		return true;
	}
	vec3 albedo;

};

class Metal : public Material
{
public:
	Metal(vec3& a) : albedo(a) {}
	virtual bool scatter(Ray& r_in, hitRecord& rec, vec3& attenuation, Ray& scattered)
	{
		vec3 reflected = reflect(normalize(r_in.D), rec.normal);
		scattered = Ray(rec.hitPoint, reflected);
		attenuation = albedo;
		return (dot(scattered.D, rec.normal) > 0);
	}
	vec3 albedo;
};


#endif
