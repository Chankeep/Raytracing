#pragma once
#include <vector>
#include <memory>
#include "hittable.h"
class hittable_list : public hittable
{
public:
	hittable_list() = default;
	hittable_list(const std::shared_ptr<hittable>& object) { add(object); }

	void add(const std::shared_ptr<hittable>& object)
	{
		objects.push_back(object);
	}

	virtual bool hit(const ray& r, hit_record& rec, double t_max, double t_min) const override;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

	std::vector<std::shared_ptr<hittable>> objects;
};

//通过循环每一个list内的对象，并使用该对象的hit函数检测对象与光线有无交点再返回到record
inline bool hittable_list::hit(const ray& r, hit_record& rec, double t_max, double t_min) const
{
	hit_record temp_rec;
	bool hit_any = false;
	double closest = t_max;
	for (const auto & object : objects)
	{
		if(object->hit(r, temp_rec, closest, t_min))
		{
			hit_any = true;
			closest = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_any;
}

inline bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const
{
	if (objects.empty()) return false;

	aabb temp_box;
	bool first_box = true;

	for (const auto& object : objects) {
		if (!object->bounding_box(time0, time1, temp_box)) return false;
		output_box = first_box ? temp_box : aabb::surrounding_box(output_box, temp_box);
		first_box = false;
	}

	return true;
}
