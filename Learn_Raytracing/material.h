#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"
#include "texture.h"

struct hit_record;

class material
{
public:
	// 散射函数,	attenuation 减弱
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const = 0;

	// 材质发光函数,非自发光物体返回（0，0，0）
	virtual color emitted(double u, double v, const point3& p) const
	{
		return color(0, 0, 0);
	}
};

// lambertian 材质类
class lambertian : public material
{
public:
	lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
	lambertian(shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override
	{
		auto scatter_direction = rec.normal + random_unit_vector();
		// 捕获散射方向
		if (scatter_direction.near_zero())
		{
			scatter_direction = rec.normal;
		}

		scattered = ray(rec.p, scatter_direction, r_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

public:
	shared_ptr<texture> albedo;	
};

// 金属 材质类
class metal : public material
{
public:
	metal(const color& a) : albedo(a) {}

	metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);

		// 模糊反射，将反射光线的尾端进行随即移动（在半径为1的求体内）
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	color albedo;
	double fuzz;	// fuzz越大，反射的光线越模糊
};

// 可以折射的材质,电介质
class dielectric : public material
{
public:
	dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override
	{
		attenuation = color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		vec3 unit_direction = unit_vector(r_in.direction());

		// 具体计算详见文档
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		// 若大于1，则不能折射，全内反射（相当于镜面反射）,否则，斯涅尔折射定律
		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
		{
			direction = reflect(unit_direction, rec.normal);
		}
		else
		{
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}

		// 出射光线
		scattered = ray(rec.p, direction, r_in.time());
		return true;
	}

private:
	// Schlick近似，当用非常陡峭的角度看玻璃，它会变成一面镜子
	static double reflectance(double cosine, double ref_idx)
	{
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);	// pow(x, y)，求x的y次幂
	}

public:
	double ir; // Index of Refraction
};

// 发光材质
class diffuse_light : public material
{
public:
	diffuse_light(shared_ptr<texture> a) : emit(a) {}
	diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override
	{
		return false;
	}

	virtual color emitted(double u, double v, const point3& p) const override
	{
		return emit->value(u, v, p);
	}

public:
	shared_ptr<texture> emit;
};

// 各向同性类，各向同性的散射函数选择均匀的随机方向
class isotropic : public material
{
public:
	isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
	isotropic(shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}


public:
	shared_ptr<texture> albedo;
};


#endif // !MATERIAL_H
