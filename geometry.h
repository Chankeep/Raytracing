#pragma once

#include<cmath>
#include<iostream>
#include<limits>
#include<random>
using std::sqrt;
using std::shared_ptr;
using std::make_shared;

class vec3 {
public:
	//构造函数
	vec3() :e{ 0,0,0 } { }
	vec3(double e0, double e1, double e2) :e{ e0, e1, e2 } { }
	//坐标
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
	//运算符
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }
	vec3& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}
	//长度相关
	double length() const {
		return sqrt(length_squared());
	}
	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	vec3& operator+(double i) ;
public:
	double e[3];
};

inline vec3& vec3::operator+(double i)
{
	e[0] += i;
	e[1] += i;
	e[2] += i;
	return *this;
}

using point3 = vec3;	//3D point
using color = vec3;		//RGB color
// vec3 Utility Functions
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}
inline vec3 operator/(const vec3& v, double t) {
	return (1 / t) * v;
}
//数量积
inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}
//叉乘
inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
inline vec3 normalize(const vec3& v) {
	return v / v.length();
}

constexpr double PI = 3.141592657;
constexpr double Infinity = std::numeric_limits<double>::infinity();

inline double degree2radian(const double degree)
{
	return degree * PI / 180.0;
}

inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}
inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}
static vec3 random()
{
	return {random_double(), random_double(), random_double()};
}

static vec3 random(double min, double max)
{
	return {random_double(min, max), random_double(min, max), random_double(min, max)};
}

inline vec3 random_in_unit_sphere()
{
	while(true)
	{
		vec3 p = random(-1, 1);
		if (p.length_squared() >= 1)
			continue;
		return p;
	}
}

inline double clamp(double max, double min, double x)
{
	if (x < min) x = min;
	if (x > max) x = max;
	return x;
}