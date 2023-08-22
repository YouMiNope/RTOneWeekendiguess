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
