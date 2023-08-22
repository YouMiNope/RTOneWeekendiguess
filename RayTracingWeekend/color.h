#pragma once

#include "vec3.h"
#include "utils.h"
#include "interval.h"

using color = vec3;

inline color blend(const color& a, const color& b, double alpha)
{
	return alpha * a + (1 - alpha) * b;
}

inline double gamma(double c, double gamma)
{
	return pow(c, 1 / gamma);
}