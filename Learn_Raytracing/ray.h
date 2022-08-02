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

	// �ڵ�orig��dir������t�����λ��
	point3 at(double t) const
	{
		return orig + t * dir;
	}

public:
	point3 orig;	//��Դ��
	vec3 dir;		//����
	double tm;		//ʱ��
};

#endif