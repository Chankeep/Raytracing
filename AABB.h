#pragma once
#include "geometry.h"
#include "ray.h"

//Axis-Aligned Bounding Boxes
class aabb
{
public:
	aabb() = default;
	aabb(const point3& a, const point3& b) : minimum(a), maximum(b){}

	point3 max_pos() const { return maximum; }
	point3 min_pos() const { return minimum; }

	bool hit(const ray& r, double t_min, double t_max) const
	{
		for (int i = 0; i < 3; i++)
		{
			auto invDir = 1.0f / r.get_dir()[i];
			auto t0 = (min_pos()[i] - r.get_origin()[i]) * invDir;
			auto t1 = (max_pos()[i] - r.get_origin()[i]) * invDir;
			if (invDir < 0.0f)
				std::swap(t0, t1);
			//循环三次，三个对面，enter最晚的就代表光线进入了box，left点最早就代表已经离开了box，必须要同时在三个对面内才算进入
			t_min = t0 > t_min ? t0 : t_min;
			t_max = t1 < t_max ? t1 : t_max;
			//有t_enter < 0,t_left < 0就代表光起点是在box内部 t是代表distance
			if (t_max <= t_min)
				return false;
		}
		return true;
	}

	static aabb surrounding_box(aabb& box1, aabb& box2);
public:
	point3 minimum;
	point3 maximum;
};

inline aabb aabb::surrounding_box(aabb& box1, aabb& box2)
{
	point3 small(std::min(box1.min_pos().x(), box2.min_pos().x()),
		std::min(box1.min_pos()[1], box2.min_pos()[1]),
		std::min(box1.min_pos()[2], box2.min_pos()[2]));
	point3 big(std::max(box1.max_pos().x(), box2.max_pos().x()),
		std::max(box1.max_pos()[1], box2.max_pos()[1]),
		std::max(box1.max_pos()[2], box2.max_pos()[2]));
	return aabb(small, big);
}
