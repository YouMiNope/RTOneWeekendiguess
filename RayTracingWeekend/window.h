#pragma once

#if defined(_WIN32)
  #include <GLFW/glfw3.h>
  #include <gl/GL.h>
#else
  #include <GL/glew.h>
  #include <GLFW/glfw3.h>
#endif

#include <iostream>

#include "buffer.h"

class window
{
public:
	GLFWwindow* m_window;

	int width;
	int height;

	pixbuf<double>& raw_buffer;

	window(int width, int height, pixbuf<double>& buf) :
		width(width),
		height(height),
		raw_buffer(buf),
		buffer(width, height, 3)
	{
		if (glfwInit() != GLFW_TRUE)
		{
			std::cout << "GLFW Initialize Failed" << std::endl;
		}
		create_window(width, height);
	}

	~window() {
		glfwTerminate();
	}

	void draw_loop(double(*post_proc_func)(double))
	{
		while (!glfwWindowShouldClose(m_window))
		{
			// do something
			if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(m_window, GLFW_TRUE);
			}
			glClear(GL_COLOR_BUFFER_BIT);

			raw_buffer.render_to(buffer, post_proc_func);
			glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)buffer.ptr());

			glfwSwapBuffers(m_window);

			glfwPollEvents();
		}
	}

private:
	pixbuf<char8_t> buffer;

	void create_window(int width, int height)
	{
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);

		m_window = glfwCreateWindow(width, height, "Ray Tracing I Guess", NULL, NULL);
		if (!m_window)
		{
			std::cout << "GLFW Windows Failed to Create" << std::endl;
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1);

		// OpenGL stuffs
		glClearColor(0.2f, 0.3f, 0.35f, 1.0f);
		glViewport(0, 0, width, height);
		
	}
};