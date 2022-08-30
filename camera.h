#pragma once
#include "geometry.h"
#include "ray.h"

vec3 up(0, 1, 0);
class camera
{
public:
	//vertical_fov为竖直方向的field of view，为角度degree
	camera(point3 lookfrom, point3 lookat, double vertical_fov, double aspect_ratio, double aperture, double focus, double time1 = 0.0, double time2 = 0.0) : lens_radius(aperture / 2), Origin(lookfrom), _time1(time1), _time2(time2)
	{
		auto theta_radian = degree2radian(vertical_fov);
		auto h = tan(theta_radian / 2);
		auto viewport_height = h * 2.0;
		auto viewport_width = viewport_height * aspect_ratio;

		LookAt(lookfrom, lookat);
		Horizontal = x * viewport_width * focus;
		Vertical = y * viewport_height * focus;
		LowerLeftCorner = Origin - Horizontal / 2 - Vertical / 2 + z * focus;

	}

	void LookAt(const point3 lookfrom, const point3 lookat)
	{
		z = normalize(lookat - lookfrom);
		x = normalize(cross(z, up));
		y = cross(x, z);
	}
	ray get_ray(double u, double v) const
	{
		vec3 rd = random_in_unit_disk() * lens_radius;
		vec3 offset = x * rd.x() + y * rd.y();
		return ray(Origin + offset, LowerLeftCorner + u * Horizontal + v * Vertical - Origin - offset, random_double(_time1, _time2));
	}

private:
	point3 LowerLeftCorner;
	point3 Horizontal;
	vec3 x, y, z;
	vec3 Vertical;
	vec3 Origin;
	double lens_radius;
	double _time1, _time2;
};

