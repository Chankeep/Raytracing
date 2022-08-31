#include "BVH.h"


//comparsion function

bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis)
{
	aabb box_a;
	aabb box_b;

	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	return box_a.min_pos()[axis] < box_b.min_pos()[axis];
}

bool box_compare_x(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
	return box_compare(a, b, 0);
}

bool box_compare_y(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
	return box_compare(a, b, 1);
}

bool box_compare_z(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
	return box_compare(a, b, 2);
}

bool BVH_node::bounding_box(double time0, double time1, aabb& output_box) const
{
	output_box = box;
	return true;
}

bool BVH_node::hit(const ray& r, hit_record& rec, double t_max, double t_min) const
{
	if (!box.hit(r, t_min, t_max))
		return false;
	
	bool hit_left = left->hit(r, rec, t_max, t_min);
	bool hit_right = right->hit(r, rec, hit_left ? rec.t : t_max, t_min);

	return hit_left || hit_right;
}

BVH_node::BVH_node(const std::vector<shared_ptr<hittable>> src_objects, size_t start, size_t end, double time0, double time1)
{
	auto objects = src_objects;
	int axis = random_int(0, 2);
	auto comparator = (axis == 0) ? box_compare_x : (axis == 1) ? box_compare_y : box_compare_z;

	size_t object_span = end - start;

	if (object_span == 1)
		left = right = objects[start];
	else if(object_span == 2)
	{
		//ÊÇtrue¾ÍÊÇÉýÐò
		if (comparator(objects[start], objects[start + 1]))
		{
			left = objects[start];
			right = objects[start + 1];
		}
		else
		{
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else
	{
		std::sort(objects.begin() + start, objects.begin() + end, comparator);
		size_t mid = start + object_span / 2;
		left = make_shared<BVH_node>(objects, start, mid, time0, time1);
		right = make_shared<BVH_node>(objects, mid, end, time0, time1);
	}

	aabb box_left, box_right;

	if (!left->bounding_box(time0, time1, box_left)
		|| !right->bounding_box(time0, time1, box_right)
		)
		std::cerr << "No bounding box in bvh_node constructor.\n";

	box = aabb::surrounding_box(box_left, box_right);
}
