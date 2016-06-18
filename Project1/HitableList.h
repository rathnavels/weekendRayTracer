#ifndef HITABLELISTH
#define HITABLELISTH

#include "Hitable.h"

class hitable_list : public hitable
{
public:
	hitable_list(){}
	hitable_list(hitable **l, int n) : list(l), list_size(n){};
	virtual bool hit(Ray, float, float, hitRecord&);
	hitable **list;
	int list_size;
};

bool hitable_list::hit(Ray ray, float tmin, float tmax, hitRecord& rec)
{

	hitRecord tempRec;
	bool hitAnything = false;
	double closest_so_far = tmax;
	for (int i = 0; i < list_size; i++)
	{
		if (list[i]->hit(ray, tmin, closest_so_far, tempRec))
		{
			hitAnything = true;
			closest_so_far = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}

#endif