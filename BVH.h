#pragma once
#include <algorithm>
#include "geometry.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"

class BVH_node : public hittable
{
public:
	BVH_node() = default;

	BVH_node(const hittable_list& list, double time0, double time1) : BVH_node(list.objects, 0, list.objects.size(), time0, time1) {}

	BVH_node(const std::vector<shared_ptr<hittable>> src_objects, size_t start, size_t end, double time0, double time1);
	bool bounding_box(double time0, double time1, aabb& output_box) const override;
	bool hit(const ray& r, hit_record& rec, double t_max, double t_min) const override;
	
	shared_ptr<hittable> left;
	shared_ptr<hittable> right;
	aabb box;
};

