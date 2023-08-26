#pragma once

#include "vec3.h"

class ray
{
private:
	point3 ori;
	vec3<double>   dir;

public:
	ray() {}
	ray(const point3& origin, const point3& direction) : ori(origin), dir(direction) {}

	point3 origin() const { return ori; }
	vec3<double> direction() const { return dir; }

	vec3<double> at(double t) const
	{
		return ori + dir * t;
	}
};

inline vec3<double> diffuse(const vec3<double>& n) {
	return (n + vec3<double>::random_sphere()) / 2.;
}

inline vec3<double> reflect(const vec3<double>& uv, const vec3<double>& n) {
	return norm(uv + 2 * dot(-uv, n) * n);
}

inline vec3<double> refract(const vec3<double>& uv, const vec3<double>& n, double etai_d_etat)
{
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3<double> r_out_perp = etai_d_etat * (uv + cos_theta * n);
	vec3<double> r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_square())) * n;
	return r_out_perp + r_out_parallel;
}