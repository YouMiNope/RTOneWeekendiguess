#pragma once

#include "vec3.h"
#include "ray.h"
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



class hitable_list : public hitable
{
private:
	std::vector<hitable*> m_list;

public:
	hitable_list() {}

	void add(hitable* hit_obj)
	{
		m_list.push_back(hit_obj);
	}

	void clear()
	{
		m_list.clear();
	}

	bool hit(const ray& r, interval range_t, hit_record& rec) const override
	{
		hit_record temp_rec;
		bool hit_rtn = false;
		for (const auto& i_obj : m_list)
		{
			if (i_obj->hit(r, range_t, temp_rec))
			{
				hit_rtn = true;
				range_t.max = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_rtn;
	}
};



class sphere : public hitable
{
private:
	vec3 posit;
	double radius;

	std::shared_ptr<material> m_material;

public:
	sphere(const vec3& position, double radius, std::shared_ptr<material> matr) : posit(position), radius(radius), m_material(matr) {}

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
		record.material = m_material;

		return true;
	}
};