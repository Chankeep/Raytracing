#pragma once

#include "hittable_list.h"
#include "aarect.h"

class box : public hittable
{
public:
	box() = default;

	box(const point3& p0, const point3& p1, shared_ptr<material> mat);

	bool hit(const ray& r, hit_record& rec, double t_max, double t_min) const override;

	bool bounding_box(double time0, double time1, aabb& output_box) const override
	{
		output_box = aabb(box_min, box_max);
		return true;
	}

private:
	point3 box_max;
	point3 box_min;
	hittable_list sides;
};

inline box::box(const point3& p0, const point3& p1, shared_ptr<material> mat)
{
	box_min = p0;
	box_max = p1;

	//xy sides
	sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat));
	sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat));
	//xz sides
	sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat));
	sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat));
	//yz sides
	sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat));
	sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat));
}

inline bool box::hit(const ray& r, hit_record& rec, double t_max, double t_min) const
{
	return sides.hit(r, rec, t_max, t_min);
}


