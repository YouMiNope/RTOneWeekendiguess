#pragma once

#include <thread>
#include <vector>
#include "hitable.h"
#include "intrin.h"
//#include "camera.h"
class camera;

class pool
{
public:
	pool() : m_cores(1) {}
	pool(uint32_t size) : m_cores(size) {}

	uint32_t set_cores(uint32_t new_cores) 
	{ 
		m_cores = new_cores > std::thread::hardware_concurrency() - 1 ? std::thread::hardware_concurrency() - 1 : new_cores;
		return m_cores;
	}

	auto get_actives() { return m_active_count.load(); }

	void activate_thread(void(camera::*p_func)(const hitable*, size_t), camera* p_cam, const hitable* world, size_t y_line)
	{
		while (m_active_count >= m_cores) {}
		m_active_count++;
		auto a_thread = std::thread(&pool::_single_func, this, p_func, p_cam, world, y_line);
		a_thread.detach();
	}

	void wait_all_return()
	{
		while (m_active_count > 0) {}
		return;
	}


private:
	uint32_t m_cores;
	std::atomic<uint32_t> m_active_count;

	void _single_func(void(camera::*p_func)(const hitable*, size_t), camera* p_cam, const hitable* world, size_t y_line)
	{
		(p_cam->*p_func)(world, y_line);
		m_active_count--;
	}
};