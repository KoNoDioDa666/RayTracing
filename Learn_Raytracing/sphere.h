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
	shared_ptr<material> mat_ptr;	// ����
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

	// ������̵Ľ�t��Ҳ�������й�ʽ���t����Ϊ�����������㣬����ֻ��Ҫ�Ҹ������ĵ�һ����
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || root > t_max)
	{
		root = (-half_b + sqrtd) / a;
		if (root<t_min || root>t_max)
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);	// ���ߺ���Ľ���
	vec3 outward_normal = (rec.p - center) / radius;	//����ķ�����(��λ����)
	rec.set_face_normal(r, outward_normal);	// ���ý���ķ�����
	get_sphere_uv(outward_normal, rec.u, rec.v); // �� rec �е� uv ��ֵ
	rec.mat_ptr = mat_ptr;

	return true;
}

// ���߿������
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