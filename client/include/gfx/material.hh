#ifndef IMU_MATERIAL_H
#define IMU_MATERIAL_H

#include <gfx/texture.hh>

class Material
{
public:
	Texture *diffuse;
	Texture *specular;
	float shininess;
	
	Material(Texture *diffuse, Texture *specular, float shininess) :
		diffuse(diffuse), specular(specular), shininess(shininess)
	{}
};

#endif //IMU_MATERIAL_H
