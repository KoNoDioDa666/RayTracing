#pragma once
#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

// 该头文件主要定义一些数学常量

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
// 
// 角度 转化为 弧度
inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}

// 随机数
inline double random_double()
{
	// [0,1)
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(double min, double max)
{
	// [min,max)
	return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

inline int random_int(int min, int max)
{
	return static_cast<int>(random_double(min, max + 1));
}

// Common Headers
#include "ray.h"
#include "vec3.h"


#endif