#pragma once
#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb
{
public:
	aabb() {}
	// 两个点 a、b 是包围盒对角线的两个顶点，两个顶点便可表示一个完整的包围盒
	aabb(const point3& a, const point3& b)
	{
		minimum = a;
		maximum = b;
	}

	point3 min() const
	{
		return minimum;
	}
	point3 max() const
	{
		return maximum;
	}

	/*bool hit(const ray& r, double t_min, double t_max) const
	{
		for (int a = 0; a < 3; ++a)
		{
			auto t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a],
						   (maximum[a] - r.origin()[a]) / r.direction()[a]);
			auto t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a],
						   (maximum[a] - r.origin()[a]) / r.direction()[a]);

			t_min = fmax(t0, t_min);
			t_max = fmin(t1, t_max);
			if (t_max <= t_min)
			{
				return false;
			}
		}
		return true;
	}*/
	bool hit(const ray& r, double t_min, double t_max) const;

public:
	point3 minimum;
	point3 maximum;
};

inline bool aabb::hit(const ray& r, double t_min, double t_max) const
{
	for (int a = 0; a < 3; ++a)
	{
		auto invD = 1.0f / r.direction()[a];
		auto t0 = (min()[a] - r.origin()[a]) * invD;
		auto t1 = (max()[a] - r.origin()[a]) * invD;

		if (invD < 0.0f)
		{
			std::swap(t0, t1);
		}
		t_min = t0 > t_min ? t0 : t_min;
		t_max = t1 < t_max ? t1 : t_max;
		//t_min = fmax(t0, t_min);
		//t_max = fmin(t1, t_max);
		if (t_min > t_max)
		{
			return false;
		}
	}

	return true;
}

// 把两个包围盒合并为一个大包围盒
aabb surrounding_box(aabb box0, aabb box1)
{
	point3 small(fmin(box0.min().x(), box1.min().x()),
				 fmin(box0.min().y(), box1.min().y()),
				 fmin(box0.min().z(), box1.min().z()));

	point3 big(fmax(box0.max().x(), box1.max().x()),
			   fmax(box0.max().y(), box1.max().y()),
			   fmax(box0.max().z(), box1.max().z()));

	return aabb(small, big);
}

#endif