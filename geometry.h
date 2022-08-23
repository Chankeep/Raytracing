#pragma once

#include <cmath>
class vec3
{
public:
	double e[3]{};
	vec3();
	vec3(double e1, double e2, double e3);
	inline double x() const;
	inline double y() const;
	inline double z() const;
	inline double r() const;
	inline double g() const;
	inline double b() const;

	inline vec3 operator+(const vec3& v) const;
	inline vec3 operator-(const vec3& v) const;
	
	inline vec3 operator/(const double t) const;
	inline vec3& operator+=(const vec3& v);
	inline vec3& operator-=(const vec3& v);
	inline vec3& operator*=(double t);
	inline vec3& operator/=(const double t);
	inline double dot(const vec3& v) const;
	inline vec3& normalize();
	inline double length() const;
	friend inline double dot(const vec3& v1, const vec3& v2);
	friend inline vec3 cross(const vec3& v1, const vec3& v2);
};

inline vec3 operator*(const vec3& v, double  t)
{
	return vec3{ v.e[0] * t, v.e[1] * t, v.e[2] * t };
}

