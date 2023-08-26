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

		m_camera.terminate();

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

			glDrawBuffer(GL_AUX0);
			glDrawPixels(m_camera.get_width(), m_camera.get_height(), GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)m_camera.out_buffer.ptr(0, 0, 0));
			glDrawBuffer(GL_BACK);

			glBindTexture(GL_TEXTURE_2D, render_tex);
			glReadBuffer(GL_AUX0);
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_camera.get_width(), m_camera.get_height());
			
			glBindTexture(GL_TEXTURE_2D, 0);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(m_window);
		}
	}

private:

	GLFWwindow* m_window;

	camera& m_camera;

	GLuint render_tex;

	void create_window(int width, int height)
	{
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);

		m_window = glfwCreateWindow(width, height, "Ray Tracing I Guess", NULL, NULL);
		if (!m_window)
		{
			std::cout << "GLFW Windows Failed to Create" << std::endl;
			return;
		}

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1);

		// OpenGL stuffs
		glClearColor(0.2f, 0.3f, 0.35f, 1.0f);
		glViewport(0, 0, width, height);

		glGenTextures(1, &render_tex);
		glBindTexture(GL_TEXTURE_2D, render_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_camera.get_width(), m_camera.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	void show_GUI()
	{
		{
			static int m_cam_width = 800;
			const size_t _slow = 1;
			const size_t _fast = 16;
			static double render_time_start = 0.;
			static double render_time = 0.;

			ImGui::Begin("Main");
			ImGui::Text("Camera");
			ImGui::InputInt("width", &m_cam_width);
			ImGui::InputScalar("sample count", ImGuiDataType_U64, &m_camera.sample_count, &_slow, &_fast);
			ImGui::InputScalar("max reflection", ImGuiDataType_U64, &m_camera.max_reflect, &_slow, &_fast);
			ImGui::InputScalar("muti-core", ImGuiDataType_U32, &m_camera.cores, &_slow);
			m_camera.cores = m_camera.render_pool.set_cores(m_camera.cores);

			ImGui::Text("Active cores: %u", m_camera.render_pool.get_actives());

			if (ImGui::Button("resize camera")) {
				m_camera.img_width = m_cam_width;
				m_camera.initilize();
				glDeleteTextures(1, &render_tex);
				glBindTexture(GL_TEXTURE_2D, render_tex);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_camera.get_width(), m_camera.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			}

			if (ImGui::Button("Render")) {
				m_camera.initilize();
				render_time_start = ImGui::GetTime();
				std::thread render_thread(&camera::render, &m_camera, &objects);
				render_thread.detach();
			}

			if (m_camera.get_remaining() != 0) {
				ImGui::Text("Remaining Lines: %zu", m_camera.get_remaining());
				render_time = ImGui::GetTime() - render_time_start;
			}
			else {
				ImGui::Text("IDLE");
			}
			ImGui::Text("Time: %.3f", render_time);

			ImGui::End();
		}
		{
			ImGui::Begin("Output", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
			ImGui::SetWindowSize(ImVec2(m_camera.get_width() + 16, m_camera.get_height() + 36));
			ImGui::Image((void*)(intptr_t)render_tex, ImVec2(m_camera.get_width(), m_camera.get_height()));
			ImGui::End();
			//ImGui::ShowDemoWindow();
		}
		{
			ImGui::Begin("Scene");
			ImGui::ColorEdit3("TopSky", &m_camera.top_sky[0]);
			ImGui::ColorEdit3("BottomSky", &m_camera.bottom_sky[0]);
			ImGui::End();
		}
	}
};