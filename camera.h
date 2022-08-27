#pragma once
#include "geometry.h"
#include "ray.h"

class camera
{
public:
	camera()
	{
		auto aspect_ratio = 16.0 / 9.0;
		auto viewport_height = 2.0;
		auto viewport_width = aspect_ratio * viewport_height;
		auto focal_length = 1.0;

		Origin = { 0,0,0 };
		Horizontal = { viewport_width,0.0,0.0 };
		Vertical = { 0.0,viewport_height,0.0 };
		LowerLeftCorner = Origin - Horizontal / 2 - Vertical / 2 - vec3(0, 0, focal_length);

	}

	ray get_ray(double u, double v) const
	{
		return ray(Origin, LowerLeftCorner + u * Horizontal + v * Vertical - Origin);
	}

private:
	point3 LowerLeftCorner;
	point3 Horizontal;
	vec3 Vertical;
	vec3 Origin;
};

