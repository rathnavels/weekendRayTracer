//#ifndef	MATERIAL_H
//#define MATERIAL_H
//
//#include "Hitable.h"
//
//class Material
//{
//public:
//	virtual bool scatter(Ray& ray, hitRecord& rec, vec3& attenuation, Ray& scattered) = 0;
//
//};
//
//class Lambertian : public Material
//{
//public:
//	Lambertian(vec3& a) : albedo(a){}
//	virtual bool scatter(Ray& ray, hitRecord& record, vec3& attenuation, Ray& scattered)
//	{
//		vec3 target = record.hitPoint + record.normal + random_in_unitSphere();
//		scattered = Ray(record.hitPoint, target - record.hitPoint);
//		attenuation = albedo;
//		return true;
//	}
//
//	vec3 albedo;
//};
//
//vec3 random_in_unitSphere()
//{
//	vec3 p;
//	//	do
//	//	{
//	p = (2.0f * vec3(((float)(rand()) / (RAND_MAX + 1)), ((float)(rand()) / (RAND_MAX + 1)), ((float)(rand()) / (RAND_MAX + 1)))) - vec3(1.0, 1.0, 1.0);
//	//} while ( (p.x)*(p.x) + (p.y)*(p.y) + (p.z)*(p.z) >= 1.0);
//
//	return p;
//}
//
//
//
//
//#endif