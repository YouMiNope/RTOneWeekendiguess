#pragma once

#include "utils.h"

class interval
{
public:
	double min;
	double max;

	interval() : min(+infinity), max(-infinity) {}
	interval(double min, double max) : min(min), max(max) {}

	inline bool contains(double x) const
	{
		return min <= x && x <= max;
	}

	inline bool surrounds(double x) const
	{
		return min < x && x < max;
	}

	inline double clamp(double x) const
	{
		return x < min ? min : x > max ? max : x;
	}
};

const static interval empty  (+infinity, -infinity);
const static interval univers(-infinity, +infinity);
const static interval one	 (0, 1);