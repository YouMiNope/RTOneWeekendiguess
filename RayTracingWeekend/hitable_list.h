#pragma once

#include "hitable.h"
#include <vector>

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