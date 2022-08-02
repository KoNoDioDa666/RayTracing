#pragma once
#ifndef HITTABLE_LIST_H
#define  HITTABLE_LIST_H

#include "hittable.h"
#include "aabb.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

// 用来保存多个object的类
class hittable_list :public hittable
{
public:
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object)
	{
		add(object);
	}

	void clear()
	{
		objects.clear();
	}
	void add(shared_ptr<hittable> object)
	{
		objects.emplace_back(object);
	}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

public:
	std::vector<shared_ptr<hittable>> objects;
};

// 判断光线 r 是否能照射到链表中的某个物体
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects)
	{
		/* 这里的object实际上是指向hittable的派生类对象的指针, 所以调用hit函数也是调用派生类中定义的函数
		* 找到与光线相交的第一个物体的点
		*/
		if (object->hit(r, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const
{
	if (objects.empty())
		return false;

	aabb temp_box;
	bool first_box = true;

	for (const auto& object : objects)
	{
		if (!object->bounding_box(time0, time1, temp_box))
			return false;
		if (first_box)
		{
			output_box = temp_box;
		}
		else
		{
			// 把所有球体放在一个包围盒中
			surrounding_box(output_box, temp_box);
		}
		first_box = false;
	}
	return true;
}

#endif