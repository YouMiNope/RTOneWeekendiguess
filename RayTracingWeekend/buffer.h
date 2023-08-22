#pragma once
#include <memory>
#include "color.h"
#include <iostream>

template <typename T>
class pixbuf
{
public:
	pixbuf() = delete;
	pixbuf(size_t width, size_t height, size_t channle) {
		size = width * height * channle;
		buf_begin_ptr = (T*)malloc(size * sizeof(T));
		buf_write_ptr = buf_begin_ptr;
		clear();
	}

	T* ptr() {
		return buf_begin_ptr;
	}

	void clear()
	{
		for (size_t i = 0; i < size; i++) {
			buf_begin_ptr[i] = 0.;
		}
	}

	void reset() {
		buf_write_ptr = buf_begin_ptr;
	}

	void aggregate_color(color _color, int sample_count)
	{
		*buf_write_ptr++ += _color[0] / sample_count;
		*buf_write_ptr++ += _color[1] / sample_count;
		*buf_write_ptr++ += _color[2] / sample_count;
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

private:
	size_t size;

	T* buf_begin_ptr;
	T* buf_write_ptr;
};