#pragma once

#include "ray.h"
#include "hitable.h"
#include "color.h"

class material
{
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& in_r, const hit_record& hit_rec, ray& out_r, color& attenuation) const = 0;
};


class basic_material : public material
{
public:
	basic_material(color albedo, double fuzzrate, double ir) : albedo(albedo), fuzzrate(fuzzrate), ir(ir) {}

	bool scatter(const ray& in_r, const hit_record& hit_rec, ray& out_r, color& attenuation) const override
	{
		vec3<double> diff_ray_dir = diffuse(-hit_rec.normal);
		auto in_ray_dir = norm(diff_ray_dir * fuzzrate + norm(in_r.direction()) * (1 - fuzzrate));
		auto refract_factor = hit_rec.is_frontface ? 1 / ir : ir;

		vec3<double> spec_ray_dir = reflect(in_ray_dir, hit_rec.normal);
		vec3<double> ray_dir;

		double cos_theta = fmin(dot(-in_ray_dir, hit_rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		if (refract_factor * sin_theta > 1.0 || reflectance(cos_theta, refract_factor) > noise::uniform())
		{
			// reflact
			ray_dir = spec_ray_dir;
		}
		else
		{
			vec3<double> frac_ray_dir = refract(in_ray_dir, hit_rec.normal, refract_factor);
			ray_dir = frac_ray_dir;
		}
		interval zero_near(0, 1e-6);
		if (zero_near.contains(ray_dir.length_square())) {
			return false;
		}

		out_r = ray(hit_rec.p, ray_dir);

		attenuation *= albedo;
		return true;
	}

private:
	color albedo;
	double fuzzrate;
	double ir;

	static double reflectance(double cosine, double ref_idx) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};