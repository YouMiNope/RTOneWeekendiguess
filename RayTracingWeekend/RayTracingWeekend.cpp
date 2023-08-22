#include "RayTracingWeekend.h"


int main()
{
	// init
	camera m_camera;
	m_camera.img_width = 800;
	m_camera.aspect_ratio = 16. / 9.;
	m_camera.sample_count = 16;
	m_camera.max_reflect = 16;
	m_camera.initilize();

	// CPU draw buffer
	pixbuf<double> raw_buffer(m_camera.get_width(), m_camera.get_height(), 3);

	// create window
	window m_window(m_camera.get_width(), m_camera.get_height(), raw_buffer);

	// material
	color blueish(0.72, 0.4, 0.1);
	auto matr_a = std::make_shared<basic_material>(color(0.4, 0.3, 0.2), 1, infinity);
	auto matr_left = std::make_shared<basic_material>(color(0.2, 0.3, 0.7), 0.05, 1.5 / 1);
	auto matr_rigt = std::make_shared<basic_material>(color(1.), 0.0, 1.5 / 1);
	auto matr_b = std::make_shared<basic_material>(color(0.3, 0.3, 0.3), 0.5, infinity);

	// scene
	hitable_list objects;
	objects.add(new sphere(vec3(0, 0, -1.5), 0.5, matr_a));
	objects.add(new sphere(vec3(-1, 0, -1.1), 0.45, matr_left));
	objects.add(new sphere(vec3(-1, 0, -1.1), -0.2, matr_left));
	objects.add(new sphere(vec3(0.5, 0, -1), 0.5, matr_rigt));
	objects.add(new sphere(vec3(0, -200.5, -1), 200, matr_b));

	// starting a draw thread so that you can see it's drawing
	std::thread render_thread(&camera::render, &m_camera, objects, raw_buffer);
	render_thread.detach();

	// post process
	auto post_func = [](double x) -> double {
		x = gamma(x, 1.8);
		x = one.clamp(x);
		x += noise::uniform() / 256;
		return x;
	};
	m_window.draw_loop(post_func);

	return 0;
}