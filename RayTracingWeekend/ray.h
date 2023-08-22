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

inline vec3 diffuse(const vec3& n) {
	return (n + vec3::random_sphere()) / 2.;
}

inline vec3 reflect(const vec3& uv, const vec3& n) {
	return norm(uv + 2 * dot(-uv, n) * n);
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_d_etat)
{
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_d_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_square())) * n;
	return r_out_perp + r_out_parallel;
}