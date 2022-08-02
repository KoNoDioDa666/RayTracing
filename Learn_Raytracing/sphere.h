#pragma once
#ifndef SPHERE_H
#define  SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
	sphere() {}
	sphere(point3 cen, double r, shared_ptr<material> m)
		: center(cen), radius(r), mat_ptr(m) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

private:
	static void get_sphere_uv(const point3& p, double& u, double& v);

public:
	point3 center;
	double radius;
	shared_ptr<material> mat_ptr;	// 材质
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	vec3 oc = r.origin() - center;
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
	vec3 outward_normal = (rec.p - center) / radius;	//交点的法向量(单位向量)
	rec.set_face_normal(r, outward_normal);	// 设置交点的法向量
	get_sphere_uv(outward_normal, rec.u, rec.v); // 给 rec 中的 uv 赋值
	rec.mat_ptr = mat_ptr;

	return true;
}

// 带边框的球体
bool sphere::bounding_box(double time0, double time1, aabb& output_box) const
{
	output_box = aabb(
		center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
	return true;
}

void sphere::get_sphere_uv(const point3& p, double& u, double& v)
{
	auto theta = acos(-p.y());
	auto phi = atan2(-p.z(), p.x()) + pi;

	u = phi / (2 * pi);
	v = theta / pi;
}

#endif // !SPHERE_H
