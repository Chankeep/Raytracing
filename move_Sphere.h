#pragma once
#include "hittable.h"

class move_Sphere : public hittable
{
public:
	move_Sphere() = default;
	move_Sphere(point3 cen1, point3 cen2, double time1, double time2, double r, shared_ptr<material> m) : center1(cen1), center2(cen2), _time1(time1), _time2(time2), radius(r), mat_ptr(m) {  }
	bool hit(const ray& r, hit_record& rec, double t_max, double t_min) const override;
	bool bounding_box(double time0, double time1, aabb& output_box) const override;
	point3 center(double time) const;
	//Property
	point3 center1;
	point3 center2;
	double _time1;
	double _time2;
	double radius;
	shared_ptr<material>mat_ptr;
private:
	static void get_sphere_uv(const point3& p, double& u, double& v)
	{
		//p:以原点为圆心的单位圆上的一个点，可以用法线表示
		//u:[0,1]映射从-x -> +z -> +x -> -z -> +x
		//v:[0,1]映射从-y -> +y, y与u无关
		auto theta = acos(-p.y());
		auto phi = atan2(-p.z(), p.x()) + PI;

		u = phi / (2 * PI);
		v = theta / PI;
	}
};

inline point3 move_Sphere::center(double time) const
{
	return center1 + (time - _time1) / (_time2 - _time1) * (center2 - center1);
}

inline bool move_Sphere::hit(const ray& r, hit_record& rec, double t_max, double t_min) const
{
	const vec3 AC = r.get_origin() - center(r.get_time());
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
	auto outward_normal = (rec.pos - center(r.get_time())) / radius;
	rec.set_face_normal(r, outward_normal);
	get_sphere_uv(outward_normal, rec.u, rec.v);
	rec.mat_ptr = mat_ptr;
	return true;
}

inline bool move_Sphere::bounding_box(double time0, double time1, aabb& output_box) const
{
	aabb box1(
		center(time0) - vec3(radius, radius, radius),
		center(time0) + vec3(radius, radius, radius));
	aabb box2(
		center(time1) - vec3(radius, radius, radius),
		center(time1) + vec3(radius, radius, radius));

	output_box = aabb::surrounding_box(box1, box2);
	return true;
}
