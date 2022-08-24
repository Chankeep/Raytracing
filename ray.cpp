#include "ray.h"

vec3 ray::get_origin() const
{
	return origin;
}

vec3 ray::get_dir() const
{
	return direction;
}

vec3 ray::at(double T) const
{
	return origin + direction * T;
}
