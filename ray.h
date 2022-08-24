#pragma once
#include "geometry.h"

class ray
{
public:
	ray();
	ray(const vec3& ori, const vec3& dir) : origin(ori), direction(dir){}
	vec3 get_origin() const;
	vec3 get_dir() const;
	vec3 at(double T) const;
	vec3 origin;
	vec3 direction;
};

