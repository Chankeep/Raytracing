#pragma once
#include "ray.h"

struct hit_record
{
	vec3 pos; // ���е�
	vec3 normal; // ���е㷨��
	double t; // ����
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
	virtual bool hit(const ray& r, hit_record& rec, double t_max, double t_min) = 0;
};