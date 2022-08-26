#pragma once
#include "ray.h"
class hittable
{
public:
	bool hit(const ray& r, const vec3& pos);
};