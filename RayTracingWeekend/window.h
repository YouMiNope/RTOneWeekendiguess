#pragma once

#if defined(_WIN32)
  #include <GLFW/glfw3.h>
  #include <gl/GL.h>
#else
  #include <GL/glew.h>
  #include <GLFW/glfw3.h>
#endif

#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "buffer.h"
#include "camera.h"

class window
{
public:

	int width;
	int height;

	hitable_list objects;

	window(int width, int height, camera& cam) :
		width(width),
		height(height),
		m_camera(cam)
	{
		if (glfwInit() != GLFW_TRUE)
		{
			std::cout << "GLFW Initialize Failed" << std::endl;
		}
		create_window(width, height);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init();
	}

	~window() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void draw_loop()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			show_GUI();

			if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(m_window, GLFW_TRUE);
			}
			glClear(GL_COLOR_BUFFER_BIT);

			//m_camera.buffer.render_to(m_camera.out_buffer, post_proc_func);
			glDrawPixels(m_camera.get_width(), m_camera.get_height(), GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)m_camera.out_buffer.ptr());

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(m_window);
		}
	}

private:

	GLFWwindow* m_window;

	camera& m_camera;

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

	void show_GUI()
	{
		static int m_cam_width = 800;
		ImGui::Begin("Main");
		ImGui::Text("Camera");
		ImGui::InputInt("width", &m_cam_width);

		if (ImGui::Button("refresh camera")) {
			m_camera.img_width = m_cam_width;
			m_camera.initilize();
		}
		if (ImGui::Button("Render")) {
			m_camera.initilize();
			std::thread render_thread(&camera::render, &m_camera, objects);
			render_thread.detach();
		}
		ImGui::End();
		//ImGui::ShowDemoWindow();
	}
};