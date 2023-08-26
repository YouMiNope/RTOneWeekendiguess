#pragma once

#include <cmath>
#include "utils.h"


template <typename T>
class vec3
{
private:
	T data[3];

public:
	vec3() : data{ 0., 0., 0. } {}
	vec3(T x, T y, T z) : data{ x, y, z } {}
	vec3(T x) : vec3(x, x, x) {}

	T x() const { return data[0]; }
	T y() const { return data[1]; }
	T z() const { return data[2]; }

	vec3<T> operator-() const {
		return vec3<T>(-data[0], -data[1], -data[2]);
	}

	T operator[](int i) const {
		return data[i];
	}

	T& operator[](int i) {
		return data[i];
	}

	vec3<T>& operator+=(const vec3<T> &b)
	{
		data[0] += b.data[0];
		data[1] += b.data[1];
		data[2] += b.data[2];
		return *this;
	}

	vec3<T>& operator*=(const vec3<T>& b)
	{
		data[0] *= b[0];
		data[1] *= b[1];
		data[2] *= b[2];
		return *this;
	}

	vec3<T>& operator*=(T t)
	{
		data[0] *= t;
		data[1] *= t;
		data[2] *= t;
		return *this;
	}

	vec3<T>& operator/=(T t) {
		return *this *= 1 / t;
	}

	T length() const {
		return sqrt(length_square());
	}

	T length_square() const {
		return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
	}

	template<typename d_T>
	operator vec3<d_T>() {
		return vec3<d_T>(static_cast<d_T>(data[0]), static_cast<d_T>(data[1]), static_cast<d_T>(data[2]));
	}

	static vec3<T> random_sphere() {
		auto alpha = noise::uniform() * 2 * pi;
		auto beta  = noise::uniform() * 2 * pi;
		return vec3<T>(cos(alpha) * cos(beta), sin(alpha), cos(alpha) * sin(beta));
	}
};

using point3 = vec3<double>;


template<typename T>
inline vec3<T> operator+(const vec3<T>& a, const vec3<T>& b) {
	return vec3<T>(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

template<typename T>
inline vec3<T> operator-(const vec3<T>& a, const vec3<T>& b) {
	return a + (-b);
}

template<typename T>
inline vec3<T> operator*(const vec3<T>& a, double t) {
	return vec3<T>(a[0] * t, a[1] * t, a[2] * t);
}

template<typename T>
inline vec3<T> operator*(double t, const vec3<T>& a) {
	return a * t;
}

template<typename T>
inline vec3<T> operator*(const vec3<T>& a, const vec3<T>& b) {
	return vec3<T>(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

template<typename T>
inline vec3<T> operator/(const vec3<T>& a, double t) {
	return a * (1 / t);
}

template<typename T>
inline T dot(const vec3<T>& a, const vec3<T>& b) {
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

template<typename T>
inline vec3<T> cross(const vec3<T>& a, const vec3<T>& b)
{
	return vec3<T>(
		a[1] * b[2] - a[2] * b[1],
		a[0] * b[2] - a[2] * b[0],
		a[0] * b[1] - a[1] * b[0]
		);
}

template<typename T>
inline vec3<T> norm(const vec3<T>& a){ 
	return a / a.length();
}
