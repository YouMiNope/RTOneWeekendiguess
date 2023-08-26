#pragma once
#include <memory>
#include "color.h"
#include <iostream>

template <typename T>
class pixbuf
{
public:
	pixbuf() {}
	pixbuf(size_t width, size_t height, size_t channel)
		: width(width),
		height(height),
		channel(channel),
		size(width * height * channel)
	{
		buf_begin_ptr = std::vector<T>(size, 0);
	}

	auto ptr(size_t x, size_t y, size_t ch) {
		return &buf_begin_ptr[(y * width + x) * 3 + ch];
	}

	void clear()
	{
		buf_begin_ptr.clear();
	}

	void aggregate_color(color _color, size_t x, size_t y, int sample_count)
	{
		*ptr(x, y, 0) += _color[0] / sample_count;
		*ptr(x, y, 1) += _color[1] / sample_count;
		*ptr(x, y, 2) += _color[2] / sample_count;
	}

	template <typename s_T>
	void pixel_process_from(pixbuf<typename s_T>& src, size_t x, size_t y, double(process_func)(double))
	{
		*ptr(x, y, 0) = static_cast<T>(process_func(*src.ptr(x, y, 0)) * 255);
		*ptr(x, y, 1) = static_cast<T>(process_func(*src.ptr(x, y, 1)) * 255);
		*ptr(x, y, 2) = static_cast<T>(process_func(*src.ptr(x, y, 2)) * 255);
	}

private:
	size_t width;
	size_t height;
	size_t channel;
	size_t size;

	std::vector<T> buf_begin_ptr;
};