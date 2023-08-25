#pragma once
#include <memory>
#include "color.h"
#include <iostream>

template <typename T>
class pixbuf
{
public:
	size_t buf_write_ptr;

	pixbuf() {}
	pixbuf(size_t width, size_t height, size_t channel)
		: width(width),
		height(height),
		channel(channel),
		size(width * height * channel)
	{
		buf_begin_ptr = std::vector<T>(size, 0);
		buf_write_ptr = 0;
		clear();
	}

	~pixbuf() {
		//free(buf_begin_ptr);
	}

	auto ptr() {
		return &buf_begin_ptr[0];
	}

	void clear()
	{
		for (size_t i = 0; i < size; i++) {
			buf_begin_ptr[i] = 0.;
		}
	}

	void reset() {
		buf_write_ptr = 0;
	}

	void aggregate_color(color _color, int sample_count)
	{
		buf_begin_ptr[buf_write_ptr++] += _color[0] / sample_count;
		buf_begin_ptr[buf_write_ptr++] += _color[1] / sample_count;
		buf_begin_ptr[buf_write_ptr++] += _color[2] / sample_count;
	}

	template <typename d_T>
	void render_to(pixbuf<typename d_T>& dest, double(process_func)(double))
	{
		//auto ceil = buf_write_ptr - buf_begin_ptr;
		static interval pix_range(0, 256);
		for (size_t i = 0; i < size; i++) {
			dest.ptr()[i] = static_cast<d_T>(pix_range.clamp(process_func(buf_begin_ptr[i]) * 255));
		}
	}

	template <typename s_T>
	void pixel_process_from(pixbuf<typename s_T>& src, double(process_func)(double))
	{
		buf_begin_ptr[buf_write_ptr++] = static_cast<T>(process_func(*(src.ptr() + src.buf_write_ptr - 3)) * 255);
		buf_begin_ptr[buf_write_ptr++] = static_cast<T>(process_func(*(src.ptr() + src.buf_write_ptr - 2)) * 255);
		buf_begin_ptr[buf_write_ptr++] = static_cast<T>(process_func(*(src.ptr() + src.buf_write_ptr - 1)) * 255);
	}

private:
	size_t width;
	size_t height;
	size_t channel;
	size_t size;

	std::vector<T> buf_begin_ptr;
};