#pragma once

#include <cmath>
#include "utils.h"


class vec3
{
private:
	double data[3];

public:
	vec3() : data{ 0., 0., 0. } {}
	vec3(double x, double y, double z) : data{ x, y, z } {}
	vec3(double x) : vec3(x, x, x) {}

	double x() const { return data[0]; }
	double y() const { return data[1]; }
	double z() const { return data[2]; }

	vec3 operator-() const {
		return vec3(-data[0], -data[1], -data[2]);
	}

	double operator[](int i) const {
		return data[i];
	}

	double& operator[](int i) {
		return data[i];
	}

	vec3& operator+=(const vec3 &b)
	{
		data[0] += b.data[0];
		data[1] += b.data[1];
		data[2] += b.data[2];
		return *this;
	}

	vec3& operator*=(const vec3& b)
	{
		data[0] *= b[0];
		data[1] *= b[1];
		data[2] *= b[2];
		return *this;
	}

	vec3& operator*=(double t)
	{
		data[0] *= t;
		data[1] *= t;
		data[2] *= t;
		return *this;
	}

	vec3& operator/=(double t) {
		return *this *= 1 / t;
	}

	double length() const {
		return sqrt(length_square());
	}

	double length_square() const {
		return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
	}

	static vec3 random_sphere() {
		auto alpha = noise::uniform() * 2 * pi;
		auto beta  = noise::uniform() * 2 * pi;
		return vec3(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * sin(beta));
	}
};

using point3 = vec3;

inline vec3 operator+(const vec3& a, const vec3& b) {
	return vec3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

inline vec3 operator-(const vec3& a, const vec3& b) {
	return a + (-b);
}

inline vec3 operator*(const vec3& a, double t) {
	return vec3(a[0] * t, a[1] * t, a[2] * t);
}

inline vec3 operator*(double t, const vec3& a) {
	return a * t;
}

inline vec3 operator*(const vec3& a, const vec3& b) {
	return vec3(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

inline vec3 operator/(const vec3& a, double t) {
	return a * (1 / t);
}

inline double dot(const vec3& a, const vec3& b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline vec3 cross(const vec3& a, const vec3& b)
{
	return vec3(
		a[1] * b[2] - a[2] * b[1],
		a[0] * b[2] - a[2] * b[0],
		a[0] * b[1] - a[1] * b[0]
		);
}

inline vec3 norm(const vec3& a){ 
	return a / a.length();
}
