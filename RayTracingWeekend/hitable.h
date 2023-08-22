#pragma once

#include "vec3.h"
#include "ray.h"
#include "interval.h"
//#include "material.h"

class hitable;
class material;

class hit_record
{
public:
	point3 p;
	vec3 normal;
	double t;
	bool is_frontface;

	std::shared_ptr<material> material;

	void set_frontface_and_normal(const ray& r, const vec3& norm)
	{
		is_frontface = dot(r.direction(), norm) < 0;
		normal = is_frontface ? norm : -norm;
	}
};

class hitable
{
public:
	virtual ~hitable() = default;

	virtual bool hit(const ray&, interval range_t, hit_record&) const = 0;
};