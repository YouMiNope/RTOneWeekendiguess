#pragma once

#include "vec3.h"

class ray
{
private:
	point3 ori;
	vec3   dir;

public:
	ray() {}
	ray(const point3& origin, const point3& direction) : ori(origin), dir(direction) {}

	point3 origin() const { return ori; }
	vec3 direction() const { return dir; }

	vec3 at(double t) const
	{
		return ori + dir * t;
	}
};