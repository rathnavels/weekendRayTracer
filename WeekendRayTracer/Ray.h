#ifndef RAY_H
#define RAY_H

#include<glm\glm.hpp>
using namespace glm;
class Ray
{
public:
	Ray(){};
	Ray( glm::vec3& o,  glm::vec3& d)
	{
		O = o;
		D = d;
	}
	vec3 origin(){ return O; }
	vec3 direction(){ return D; }
	vec3 point_at_parameter(float t){ return O + t*D; }

	vec3 O;
	vec3 D;



};



#endif