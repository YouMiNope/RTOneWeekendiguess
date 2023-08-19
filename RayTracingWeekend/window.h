#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <gl/GL.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <iostream>

#include "utils.h"

class window
{
public:
	GLFWwindow* m_window;

	int width;
	int height;

	pixbuf buffer;

	window(int width, int height, pixbuf buf) : width(width), height(height), buffer(buf)
	{
		if (glfwInit() != GLFW_TRUE)
		{
			std::cout << "GLFW Initialize Failed" << std::endl;
		}
		create_window(width, height);
	}

	~window() 
	{
		glfwTerminate();
	}

	void draw_loop()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			// do something
			if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(m_window, GLFW_TRUE);
			}
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawPixels(width, height, GL_RGB, GL_FLOAT, (GLvoid*)buffer);

			glfwSwapBuffers(m_window);

			glfwPollEvents();
		}
	}

private:

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

		// OpenGL stuffs
		glClearColor(0.2f, 0.3f, 0.35f, 1.0f);
		glViewport(0, 0, width, height);
	}
};