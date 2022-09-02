#pragma once
#include "hittable.h"

class Sphere : public hittable
{
public:
	Sphere() = default;
	Sphere(const vec3& ct, double r, shared_ptr<material> m) : center(ct), radius(r), mat_ptr(std::move(m)) {}
	bool hit(const ray& r, hit_record& rec, double t_max, double t_min) const override;
	bool bounding_box(double time0, double time1, aabb& output_box) const override;

	point3 center;
	double radius;
	shared_ptr<material> mat_ptr;
private:
	static void get_sphere_uv(const point3& p, double& u, double& v)
	{
		//p:��ԭ��ΪԲ�ĵĵ�λԲ�ϵ�һ���㣬�����÷��߱�ʾ
		//u:[0,1]ӳ���-x -> +z -> +x -> -z -> +x
		//v:[0,1]ӳ���-y -> +y, y��u�޹�
		auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + PI;

		u = phi / (2 * PI);
		v = theta / PI;
	}
};

inline bool Sphere::hit(const ray& r, hit_record& rec, double t_max, double t_min) const
{
	const vec3 AC = r.get_origin() - center;
	const double a = dot(r.get_dir(), r.get_dir());
	const double half_b = dot(r.get_dir(), AC);
	const double c = dot(AC, AC) - radius * radius;
	const double discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.pos = r.at(rec.t);
	auto outward_normal = (rec.pos - center) / radius;
	rec.set_face_normal(r, outward_normal);
	get_sphere_uv(outward_normal, rec.u, rec.v);
	rec.mat_ptr = mat_ptr;
	return true;
}

inline bool Sphere::bounding_box(double time0, double time1, aabb& output_box) const
{
	output_box = aabb(
		center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
	return true;
}
