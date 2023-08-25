#pragma once
#include <limits>
#include <random>

const double pi = 3.1415926535897932384626;
const double infinity = std::numeric_limits<double>::infinity();

inline double to_radians(double degrees)
{
	return pi * degrees / 180.;
}

namespace noise
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<double> rand_uniform;
	static std::normal_distribution<double> rand_gaus;

	inline double uniform()
	{
		return rand_uniform(e);
	}

	inline double uniform(double min, double max)
	{
		return uniform() * (max - min) + min;
	}

	inline double normal()
	{
		return rand_gaus(e);
	}

	inline double normal(double mu, double sigma)
	{
		return normal() * sigma + mu;
	}
}



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

const static interval empty(+infinity, -infinity);
const static interval univers(-infinity, +infinity);
const static interval one(0, 1);