#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera
{
public:
	camera(
		point3 lookfrom,
		point3 lookat,
		vec3 vup,	// 摄像机向上的方向
		double vfov,	//vertical field-of-view in degrees，垂直视野角度
		double aspect_ratio,	// 宽高比
		double aperture,
		double focus_dist,
		double _time0 = 0,
		double _time1 = 0
	)
	{
		// 这是正交投影？？？
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		origin = lookfrom;
		
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

		lens_radius = aperture / 2;

		time0 = _time0;
		time1 = _time1;

	}

	// 散焦模糊
	ray get_ray(double s, double t)
	{
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() * rd.y();

		return ray(
			origin + offset,
			lower_left_corner + s * horizontal + t * vertical - origin - offset,
			random_double(time0, time1)
		);
	}

private:
	point3 origin;
	point3 lower_left_corner;
	vec3 horizontal;	//水平
	vec3 vertical;	//垂直
	vec3 u, v, w;
	double lens_radius;
	double time0, time1;	// 摄像机打开/关闭时间
};

#endif