#pragma once
#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"
#include "hittable.h"
#include "aabb.h"

class moving_sphere : public hittable
{
public:
	moving_sphere() {}
	moving_sphere(
		point3 cen0, point3 cen1, double _time0, double _time1, double r, shared_ptr<material> m)
		: center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
	{};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

	point3 center(double time) const;

public:
	point3 center0, center1;
	double time0, time1;
	double radius;
	shared_ptr<material> mat_ptr;
};

bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	vec3 oc = r.origin() - center(r.time());
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	if (discriminant < 0)
		return false;
	auto sqrtd = sqrt(discriminant);

	// 求出方程的解t，也就是书中公式里的t，因为可能有两个点，所以只需要找刚碰到的第一个点
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || root > t_max)
	{
		root = (-half_b + sqrtd) / a;
		if (root<t_min || root>t_max)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);	// 光线和球的交点
	vec3 outward_normal = (rec.p - center(r.time())) / radius;	//交点的法向量(单位向量)
	rec.set_face_normal(r, outward_normal);	// 设置交点的法向量
	rec.mat_ptr = mat_ptr;

	return true;
}

bool moving_sphere::bounding_box(double _time0, double _time1, aabb& output_box) const
{
	aabb box0(
		center(_time0) - vec3(radius, radius, radius),
		center(_time0) + vec3(radius, radius, radius));
	aabb box1(
		center(_time1) - vec3(radius, radius, radius),
		center(_time1) + vec3(radius, radius, radius));
	// 把移动球体的起始位置和停止位置的包围盒合并
	output_box = surrounding_box(box0, box1);
	return true;
}

point3 moving_sphere::center(double time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

#endif