#ifndef HITABLE_H
#define HITABLE_H

#include "Ray.h"

class Material;

struct hitRecord
{
	float t;
	vec3 hitPoint;
	vec3 normal;
	Material *matPtr;
};



vec3 random_in_unitSphere()
{
	vec3 p;
	p = (2.0f * vec3(((float)(rand()) / (RAND_MAX + 1)), ((float)(rand()) / (RAND_MAX + 1)), ((float)(rand()) / (RAND_MAX + 1)))) - vec3(1.0, 1.0, 1.0);
	return p;
}

vec3 reflect(vec3 V, vec3 N)
{
	return V - (float)(2 * (dot(V, N))) * N;
}

bool refract(vec3& v, vec3& normal, float IOR, vec3& refracted)
{
	float vLen = v.length();
	vec3 V = v / vLen;
	float NdotV = dot(V, normal);
	float rootPart = 1 - IOR*IOR*(1 - NdotV*NdotV);
	if (rootPart > 0)
	{
		vec3 refracted = IOR * (V - normal * NdotV) - sqrt(rootPart) * normal;
		return true;
	}
	else
		return false;
}


class hitable
{
public:
	virtual bool hit(Ray ray, float t_min, float t_max, hitRecord& rec) = 0;
};


class Material
{
public:
	virtual bool scatter(Ray& ray, hitRecord& rec, vec3& attenuation, Ray& scattered) = 0;

};

class Lambertian : public Material
{
public:
	Lambertian(vec3& a) : albedo(a){}
	virtual bool scatter(Ray& ray, hitRecord& record, vec3& attenuation, Ray& scattered)
	{
		vec3 target = record.hitPoint + record.normal + random_in_unitSphere();
		scattered = Ray(record.hitPoint, target - record.hitPoint);
		attenuation = albedo;
		return true;
	}

	vec3 albedo;
};

class Metal : public Material
{
public:
	Metal(vec3& a) : albedo(a){}
	virtual bool scatter(Ray& ray_in, hitRecord& record, vec3& attenuation, Ray& scattered)
	{
		vec3 reflected = reflect(normalize(ray_in.D), record.normal);
		scattered = Ray(record.hitPoint, reflected);
		attenuation = albedo;
		return (dot(scattered.D, record.normal) > 0);
	}
	vec3 albedo;
};


class Glossy : public Material
{
public:
	Glossy(vec3& a, float fuss) : albedo(a), Fuss(fuss){}
	virtual bool scatter(Ray& ray_in, hitRecord& record, vec3& attenuation, Ray& scattered)
	{
		vec3 reflected = reflect(normalize(ray_in.D), record.normal);
		reflected = reflected + Fuss * random_in_unitSphere();
		scattered = Ray(record.hitPoint, reflected);
		attenuation = albedo;
		return (dot(scattered.D, record.normal) > 0);
	}
	vec3 albedo;
	float Fuss;
};

class DiElectric : public Material
{
public:
	DiElectric(float ior) : IOR(ior){}
	virtual bool scatter(Ray &ray_in, hitRecord& record, vec3& attenuation, Ray& scattered)
	{
		vec3 outwardNormal;
		float ni_over_nr;
		vec3 reflected = reflect(ray_in.D, record.normal);
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;
		if (dot(ray_in.D, record.normal) > 0)
		{
			outwardNormal = -record.normal;
			ni_over_nr = IOR;
		}
		else
		{
			outwardNormal = record.normal;
			ni_over_nr = 1 / IOR;
		}
		if (refract(ray_in.D, outwardNormal, ni_over_nr, refracted))
		{
			scattered = Ray(record.hitPoint, refracted);
		}
		else
		{
			scattered = Ray(record.hitPoint, reflected);
			return false;
		}
		return true;
		
	}

	float IOR;

	
};




#endif