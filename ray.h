#pragma once
#include "geometry.h"

class ray {
public:
	ray() = default;
	ray(const point3& origin, const vec3& direction) :orig(origin), dir(direction) { }

	point3 get_origin() const { return orig; }
	vec3 get_dir() const { return dir; }

	point3 at(double t) const {
		return orig + t * dir;
	}
public:
	point3 orig;
	vec3 dir;
};

