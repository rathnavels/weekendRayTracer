#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
public:
	virtual vec3 value(float u, float v, vec3& p) = 0;

};

class constant_texture : public Texture
{
public:
	constant_texture(){}
	constant_texture(vec3 c): color(c){}
	virtual vec3 value(float u, float v, vec3& p)
	{
		return color;
	}
	vec3 color;
};

class checker_texture : public Texture
{
public:
	checker_texture() {}
	checker_texture(Texture *t0, Texture *t1, float size) : even(t0), odd(t1), checker_size(size) {}
	virtual vec3 value(float u, float v, vec3&p)
	{
		float sines = sin(checker_size * p.x)*sin(checker_size * p.y)*sin(checker_size * p.z);
		if (sines < 0)
			return even->value(u, v, p);
		else
			return odd->value(u, v, p);
	}
	Texture *odd;
	Texture *even;
	float checker_size;
};

#endif