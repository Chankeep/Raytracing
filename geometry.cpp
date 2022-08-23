#include "geometry.h"

vec3::vec3()
{
	e[0] = 0; e[1] = 0; e[2] = 0;
}

vec3::vec3(const double e1, const double e2, const double e3)
{
	e[0] = e1; e[1] = e2; e[2] = e3;
}

double vec3::x() const
{
	return e[0];
}

double vec3::y() const
{
	return e[1];
}

double vec3::z() const
{
	return e[2];
}

double vec3::r() const
{
	return e[0];
}

double vec3::g() const
{
	return e[1];
}

double vec3::b() const
{
	return e[2];
}

vec3 vec3::operator+(const vec3& v) const
{
	return {e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]};
}

vec3 vec3::operator-(const vec3& v) const
{
	return { e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2] };
}


vec3 vec3::operator/(const double t) const
{
	return { e[0] / t, e[1] / t,e[2] / t };
}

vec3& vec3::operator/=(const double t)
{
	e[0] /= t;
	e[1] /= t;
	e[2] /= t;
	return *this;
}

vec3& vec3::operator+=(const vec3& v)
{
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

vec3& vec3::operator-=(const vec3& v)
{
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

vec3& vec3::operator*=(const double t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

double vec3::dot(const vec3& v) const
{
	return e[0] * v.e[0] + e[1] * v.e[1] + e[2] * v.e[2];
}

vec3& vec3::normalize()
{
	*this /= std::sqrt(length());
	return *this;
}

double vec3::length() const
{
	return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

double dot(const vec3& v1, const vec3& v2)
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

vec3 cross(const vec3& v1, const vec3& v2)
{
	return { v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1], v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2], v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0] };
}

