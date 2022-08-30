#pragma once
#include "hittable.h"

class Sphere : public hittable
{
public:
	Sphere() = default;
	Sphere(const vec3& ct, double r, shared_ptr<material> m) : center(ct), radius(r), mat_ptr(m) {}
	virtual bool hit(const ray& r, hit_record& rec, double t_max, double t_min) override;

	point3 center;
	double radius;
	shared_ptr<material> mat_ptr;
};

inline bool Sphere::hit(const ray& r, hit_record& rec, double t_max, double t_min)
{
	const vec3 AC = r.get_origin() - center;
	const double a = dot(r.get_dir(), r.get_dir());
	const double half_b = dot(r.get_dir(), AC);
	const double c = dot(AC, AC) - radius * radius;
	const double discriminant = half_b * half_b - a * c;
	if (discriminant > 0)
	{
		const double root = sqrt(discriminant);
		double t = (-half_b - root) / a;
		if (t > t_min && t < t_max)
		{
			rec.pos = r.at(t);
			rec.t = t;
			rec.normal = (rec.pos - center) / radius;
			rec.set_face_normal(r, rec.normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
		t = (-half_b + root) / a;
		if (t > t_min && t < t_max)
		{
			rec.pos = r.at(t);
			rec.t = t;
			const vec3 outward_normal = (rec.pos - center) / radius;
			rec.set_face_normal(r,outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}
