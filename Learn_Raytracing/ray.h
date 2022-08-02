#pragma once
#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
	ray() {}
	ray(const point3& origin, const vec3& direction, double time = 0.0)
		: orig(origin), dir(direction), tm(time) {};

	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }
	double time() const { return tm; }

	// 在点orig沿dir方向走t距离的位置
	point3 at(double t) const
	{
		return orig + t * dir;
	}

public:
	point3 orig;	//光源点
	vec3 dir;		//方向
	double tm;		//时间
};

#endif