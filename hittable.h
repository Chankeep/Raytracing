#pragma once
#include <utility>

#include "AABB.h"
#include "ray.h"
class material;

struct hit_record
{
	vec3 pos; // 击中点
	vec3 normal; // 击中点法线
	shared_ptr<material> mat_ptr;
	double t; // 距离
	//texture coordinates
	double u;
	double v;
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

class translate : public hittable
{
public:
	translate() = default;
	translate(shared_ptr<hittable> object, const vec3& offset) : _offset(offset), _object(std::move(object)) {}

	bool hit(const ray& r, hit_record& rec, double t_max, double t_min) const override;
	bool bounding_box(double time0, double time1, aabb& output_box) const override;


	vec3 _offset;
	shared_ptr<hittable> _object;

};

inline bool translate::hit(const ray& r, hit_record& rec, double t_max, double t_min) const
{
	ray moved_r(r.get_origin() - _offset, r.get_dir(), r.get_time());
	if (!_object->hit(moved_r, rec, t_max, t_min))
		return false;

	rec.pos += _offset;
	rec.set_face_normal(moved_r, rec.normal);
	return true;
}

inline bool translate::bounding_box(double time0, double time1, aabb& output_box) const
{
	if (!_object->bounding_box(time0, time1, output_box))
		return false;

	output_box = aabb(output_box.min_pos() + _offset, output_box.max_pos() + _offset);
	return true;
}

class rotate_y : public hittable
{
public:
	rotate_y() = default;

	rotate_y(shared_ptr<hittable> _object, double _angle);

	bool bounding_box(double time0, double time1, aabb& output_box) const override
	{
		output_box = bbox;
		return hasbox;
	}

	bool hit(const ray& r, hit_record& rec, double t_max, double t_min) const override;

	shared_ptr<hittable> object;
	double sine;
	double cosine;
	bool hasbox;
	aabb bbox;
};

inline rotate_y::rotate_y(shared_ptr<hittable> _object, double _angle)
{
	auto radian = degree2radian(_angle);

	sine = sin(radian);
	cosine = cos(radian);

	object = _object;

	hasbox = object->bounding_box(0, 1, bbox);

	point3 minp(Infinity, Infinity, Infinity);
	point3 maxp(-Infinity, -Infinity, -Infinity);

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
			{
				auto x = i * bbox.max_pos().x() + (1 - i) * bbox.min_pos().x();
				auto y = j * bbox.max_pos().y() + (1 - j) * bbox.min_pos().y();
				auto z = k * bbox.max_pos().z() + (1 - k) * bbox.min_pos().z();

				auto _x = cosine * x + sine * z;
				auto _z = -sine * x + cosine * z;

				vec3 newvec(_x, y, _z);

				for(int c=0; c<3 ;c++)
				{
					minp[c] = std::min(minp[c], newvec[c]);
					maxp[c] = std::max(maxp[c], newvec[c]);
				}
			}

	bbox = aabb(minp, maxp);
}

inline bool rotate_y::hit(const ray& r, hit_record& rec, double t_max, double t_min) const
{
	auto origin = r.get_origin();
	auto direction = r.get_dir();

	origin[0] = cosine * origin[0] - sine * origin[2];
	origin[2] = sine * origin[0] + cosine * origin[2];

	direction[0] = cosine * direction[0] - sine * direction[2];
	direction[2] = sine * direction[0] + cosine * direction[2];

	ray rotate_r(origin, direction, r.get_time());

	if (!object->hit(rotate_r, rec, t_max, t_min))
		return false;

	auto p = rec.pos;
	auto n = rec.normal;

	p[0] = cosine * p[0] + sine * p[2];
	p[2] = -sine * p[0] + cosine * p[2];

	n[0] = cosine * n[0] + sine * n[2];
	n[2] = -sine * n[0] + cosine * n[2];

	rec.pos = p;
	rec.set_face_normal(rotate_r, n);

	return true;
}

