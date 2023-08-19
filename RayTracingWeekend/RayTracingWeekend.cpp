#include "RayTracingWeekend.h"


int main()
{
	// init
	camera m_camera;
	m_camera.img_width = 400;
	m_camera.aspect_ratio = 16. / 9.;
	m_camera.initilize();

	// CPU draw buffer
	pixbuf output_buffer = nullptr;
	output_buffer = (pixbuf)malloc(m_camera.get_width() * m_camera.get_height() * 3 * sizeof(float));
	if (output_buffer)
	{
		for (size_t i = 0; i < m_camera.get_width() * m_camera.get_height() * 3; i++)
		{
			output_buffer[i] = 0.f;
		}
	}

	// create window
	window m_window(m_camera.get_width(), m_camera.get_height(), output_buffer);

	// scene
	hitable_list objects;
	objects.add(new sphere(vec3(0, 0, -1), 0.5));
	objects.add(new sphere(vec3(0, -100.5, -1), 100));

	// starting a draw thread so that you can see it's drawing
	std::thread render_thread(&camera::render, &m_camera, objects, output_buffer);
	render_thread.detach();

	m_window.draw_loop();

	return 0;
}