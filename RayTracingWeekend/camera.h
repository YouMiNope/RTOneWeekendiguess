#pragma once

#include "utils.h"
#include "color.h"
#include "hitable.h"
#include "hitable_list.h"

#include <iostream>
#include <format>
#include <random>

class camera
{
public:
	point3 camera_pos = point3(0, 0, 0);
	vec3 camera_front = vec3(0, 0, -1);

	double aspect_ratio = 4.0 / 3.0;
	double foucus = 1.;

	int img_width = 800;
	size_t sample_count = 16;
	size_t max_reflect = 8;

	camera() { }

	double get_width() { return img_width; }
	double get_height() { return img_height; }

	void render(const hitable& world, pixbuf buffer)
	{
		pixbuf _tmp_buf = buffer;
		for (size_t samp = 0; samp < sample_count; samp++)
		{
			for (size_t y = 0; y < img_height; ++y)
			{
				std::clog << "\rRemaining Lines: " << ((sample_count - samp) * img_height - y) << " " << std::flush;
				for (size_t x = 0; x < img_width; ++x)
				{
					ray scn_ray = screen_ray(x, y);
					color current_color = ray_color(scn_ray, world);

					_tmp_buf = write_color(_tmp_buf, current_color, sample_count);
				}
			}
			_tmp_buf = buffer;
		}
		std::clog << "\rColor Antialiasing...       " << std::flush;
		for (size_t y = 0; y < img_height; ++y)
		{
			for (size_t x = 0; x < img_width; ++x)
			{
				color color_noise(noise::uniform(), noise::uniform(), noise::uniform());
				_tmp_buf = write_color(_tmp_buf, color_noise, 256);
			}
		}
		std::clog << "\rDone                      " << std::endl;
	}

	void initilize()
	{
		// viewport
		img_height = static_cast<int>(img_width / aspect_ratio);
		img_height = img_height >= 1 ? img_height : 1;

		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * ((double)img_width / img_height);

		vec3 viewport_u(viewport_width, 0, 0);
		vec3 viewport_v(0, viewport_height, 0);

		pixel_delta_u = viewport_u / img_width;
		pixel_delta_v = viewport_v / img_height;

		viewport_bottom_left = camera_pos + camera_front * foucus - viewport_u / 2 - viewport_v / 2;
		pixel_start_pos = viewport_bottom_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

private:
	int img_height = 600;

	vec3 viewport_bottom_left;
	vec3 pixel_start_pos;

	vec3 pixel_delta_u;
	vec3 pixel_delta_v;

	color ray_color(const ray& r, const hitable& hitable_objects)
	{
		hit_record m_hit_record;

		ray active_ray = r;
		for (size_t i_ray = 0; i_ray < max_reflect; i_ray++)
		{
			bool hitted = hitable_objects.hit(active_ray, interval(0, infinity), m_hit_record);
			if (hitted)
			{
				active_ray = ray(m_hit_record.p, norm(m_hit_record.normal + vec3::random_sphere()));
				m_hit_record.decay *= 0.5;
				m_hit_record.hit_count += 1;
			}
			else
			{
				break;
			}
		}

		auto norm_dir = norm(active_ray.direction());
		auto alpha = (norm_dir.y() + 1) / 2;

		return blend(color(0.02, 0.02, 0.7), color(0.5, 0.6, 0.65), alpha) * m_hit_record.decay;
	}

	inline ray screen_ray(size_t x, size_t y) const
	{
		auto _pixel_center = pixel_start_pos + x * pixel_delta_u + y * pixel_delta_v;
		auto _pixel_mutisamp = _pixel_center + get_ray_shake();

		auto _dir = _pixel_mutisamp - camera_pos;
;
		return ray(camera_pos, _dir);
	}

	inline vec3 get_ray_shake() const
	{
		return noise::uniform(-0.5, 0.5) * pixel_delta_u + noise::uniform(-0.5, 0.5) * pixel_delta_v;
	}
};