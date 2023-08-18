#pragma once

#include "vec3.h"
#include "utils.h"
#include "interval.h"

using color = vec3;

pixbuf write_color(pixbuf buf, color _color, int sample_count)
{
	static const interval color_range(0, 1.);
	*buf++ += static_cast<float>(color_range.clamp(_color[0] / sample_count));
	*buf++ += static_cast<float>(color_range.clamp(_color[1] / sample_count));
	*buf++ += static_cast<float>(color_range.clamp(_color[2] / sample_count));
	return buf;
}

inline color blend(const color& a, const color& b, double alpha)
{
	return alpha * a + (1 - alpha) * b;
}