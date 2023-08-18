#pragma

#include "vec3.h"
#include "hitable.h"

class sphere : public hitable
{
private:
	vec3 posit;
	double radius;

public:
	sphere(const vec3& position, double radius) : posit(position), radius(radius) {}

	vec3 get_posit() const { return posit; }
	double get_radius() const { return radius; }

	bool hit(const ray& r, interval range_t, hit_record& record) const override
	{
		auto a_minus_c = r.origin() - get_posit();

		auto a = r.direction().length_square();
		auto hb = dot(r.direction(), a_minus_c);
		auto c = a_minus_c.length_square() - get_radius() * get_radius();

		auto discriminent = hb * hb - a * c;
		if (discriminent < 0)
		{
			return false;
		}
		auto t = (-hb - sqrt(discriminent)) / a;
		if (!range_t.contains(t))
		{
			t = (-hb + sqrt(discriminent)) / a;
			if (!range_t.contains(t))
			{
				return false;
			}
		}
		record.t = t;
		record.p = r.at(t);
		auto normal = (record.p - posit) / radius;
		record.set_frontface_and_normal(r, normal);

		return true;
	}
};