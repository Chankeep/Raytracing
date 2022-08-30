#pragma once
#include "geometry.h"

class ray {
public:
	ray() = default;
	ray(const point3& origin, const vec3& direction, double time = 0.0) :orig(origin), dir(direction), _time(time) { }

	point3 get_origin() const { return orig; }
	vec3 get_dir() const { return dir; }
	double get_time() const { return _time; }

	point3 at(double t) const {
		return orig + t * dir;
	}
public:
	point3 orig;
	vec3 dir;
	double _time;
};

