#pragma once
#include "AABB.h"
#include "ray.h"
class material;

struct hit_record
{
	vec3 pos; // 击中点
	vec3 normal; // 击中点法线
	shared_ptr<material> mat_ptr;
	double t; // 距离
	bool front_face;

	inline void set_face_normal(const ray& r, const vec3& outward_normal)
	{
		front_face = dot(outward_normal, r.get_dir()) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};
class hittable
{
public:
	virtual bool hit(const ray& r, hit_record& rec, double t_max, double t_min) const = 0;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};