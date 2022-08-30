#pragma once
#include "hittable.h"

class move_Sphere : public hittable
{
public:
	move_Sphere() = default;
	move_Sphere(point3 cen1, point3 cen2, double time1, double time2, double r, shared_ptr<material> m) : center1(cen1), center2(cen2), _time1(time1), _time2(time2), radius(r), mat_ptr(m) {  }
	bool hit(const ray& r, hit_record& rec, double t_max, double t_min) override;

	point3 center(double time) const;
	//Property
	point3 center1;
	point3 center2;
	double _time1;
	double _time2;
	double radius;
	shared_ptr<material>mat_ptr;
};

inline point3 move_Sphere::center(double time) const
{
	return center1 + (time - _time1) / (_time2 - _time1) * (center2 - center1);
}

inline bool move_Sphere::hit(const ray& r, hit_record& rec, double t_max, double t_min)
{
	const vec3 AC = r.get_origin() - center(r.get_time());
	const double a = dot(r.get_dir(), r.get_dir());
	const double half_b = dot(r.get_dir(), AC);
	const double c = dot(AC, AC) - radius * radius;
	const double discriminant = half_b * half_b - a * c;
	if(discriminant > 0)
	{
		const double root = sqrt(discriminant);
		double t = (-half_b - root) / a;
		if (t > t_min && t < t_max)
		{
			rec.pos = r.at(t);
			rec.t = t;
			rec.normal = (rec.pos - center(r.get_time())) / radius;
			rec.set_face_normal(r, rec.normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
		t = (-half_b + root) / a;
		if (t > t_min && t < t_max)
		{
			rec.pos = r.at(t);
			rec.t = t;
			rec.normal = (rec.pos - center(r.get_time())) / radius;
			rec.set_face_normal(r, rec.normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}
