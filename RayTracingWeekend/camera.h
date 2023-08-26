#pragma once

#include "utils.h"
#include "color.h"
#include "hitable.h"
#include "buffer.h"

#include <iostream>
#include <format>
#include <random>
#include "material.h"
#include "parallelize.h"

class camera
{
public:
	pixbuf<double> buffer;
	pixbuf<char8_t> out_buffer;
	double(*post_proc_func)(double);

	// TODO
	point3 camera_pos = point3(0, 0, 0);
	vec3<double> camera_front = vec3<double>(0, 0, -1);

	double aspect_ratio = 4.0 / 3.0;
	double foucus = 1.;
	int img_width = 800;

	size_t sample_count = 16;
	size_t max_reflect = 8;

	// muti-core
	uint32_t cores = 4;
	pool render_pool = pool(4);

	// sky
	color3f top_sky = color3f(0.6, 0.4, 0.5);
	color3f bottom_sky = color3f(0.02, 0.02, 0.3);

	camera() { }
	~camera() { terminate(); }

	double get_width() { return img_width; }
	double get_height() { return img_height; }
	size_t get_remaining() { return m_remaining_lines; }

	void render(const hitable* world)
	{
		m_render_terminate = false;
		for (size_t samp = 0; samp < sample_count; samp++)
		{
			for (size_t y = 0; y < img_height; ++y)
			{
				m_remaining_lines = ((sample_count - samp) * img_height - y);
				if (m_render_terminate) 
				{
					m_remaining_lines = 0;
					return;
				}
				
				render_pool.activate_thread(&camera::render_line, this, world, y);
			}
		}
		m_remaining_lines = 0;
	}

	void render_line(const hitable* world, size_t y_line)
	{
		for (size_t x = 0; x < img_width; ++x)
		{
			ray scn_ray = screen_ray(x, y_line);
			color current_color = ray_color(scn_ray, world);

			buffer.aggregate_color(current_color, x, y_line, sample_count);
			out_buffer.pixel_process_from(buffer, x, y_line, post_proc_func);
		}
	}

	void initilize()
	{
		// viewport
		terminate();

		img_height = static_cast<int>(img_width / aspect_ratio);
		img_height = img_height >= 1 ? img_height : 1;

		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * ((double)img_width / img_height);

		vec3<double> viewport_u(viewport_width, 0, 0);
		vec3<double> viewport_v(0, viewport_height, 0);

		pixel_delta_u = viewport_u / img_width;
		pixel_delta_v = viewport_v / img_height;

		viewport_top_left = camera_pos + camera_front * foucus - viewport_u / 2 + viewport_v / 2;
		pixel_start_pos = viewport_top_left + 0.5 * (pixel_delta_u - pixel_delta_v);

		buffer = pixbuf<double>(img_width, img_height, 3);
		out_buffer = pixbuf<char8_t>(img_width, img_height, 3);
	}

	void terminate()
	{
		m_render_terminate = true;
		render_pool.wait_all_return();
	}

private:
	int img_height = 600;

	vec3<double> viewport_top_left;
	vec3<double> pixel_start_pos;

	vec3<double> pixel_delta_u;
	vec3<double> pixel_delta_v;

	size_t m_remaining_lines = 0;

	bool m_render_terminate = false;

	color ray_color(const ray& r, const hitable* hitable_objects)
	{
		hit_record m_hit_record;
		ray active_ray = r;
		vec3<double> decay(1.);

		for (size_t i_ray = 0; i_ray < max_reflect; i_ray++)
		{
			bool hitted = hitable_objects->hit(active_ray, interval(0.00001, infinity), m_hit_record);
			if (hitted)
			{
				if (!m_hit_record.material->scatter(active_ray, m_hit_record, active_ray, decay)) {
					decay = vec3<double>(0);
					break;
				}
				continue;
			}
			else
			{
				break;
			}
		}

		auto norm_dir = norm(active_ray.direction());
		auto alpha = (norm_dir.y() + 1) / 2;

		return (vec3<double>)blend(top_sky, bottom_sky, alpha) * decay;
	}

	ray screen_ray(size_t x, size_t y) const
	{
		auto _pixel_center = pixel_start_pos + x * pixel_delta_u - y * pixel_delta_v;
		auto _pixel_mutisamp = _pixel_center + get_ray_shake();

		auto _dir = _pixel_mutisamp - camera_pos;
;
		return ray(camera_pos, _dir);
	}

	vec3<double> get_ray_shake() const
	{
		return noise::uniform(-0.5, 0.5) * pixel_delta_u + noise::uniform(-0.5, 0.5) * pixel_delta_v;
	}

};