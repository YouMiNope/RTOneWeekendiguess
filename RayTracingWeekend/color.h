#pragma once

#include "vec3.h"
#include "utils.h"

using color = vec3<double>;
using color3f = vec3<float>;

inline color blend(const color& a, const color& b, double alpha)
{
	return alpha * a + (1 - alpha) * b;
}

inline color3f blend(const color3f& a, const color3f& b, double alpha)
{
	return alpha * a + (1 - alpha) * b;
}

inline double gamma(double c, double gamma)
{
	return pow(c, 1 / gamma);
}