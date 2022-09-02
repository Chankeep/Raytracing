#pragma once

#include "geometry.h"
#include "ray.h"
#include "hittable.h"
#include "texture.h"

struct hit_record;

class material
{
public:
	virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
	explicit lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
	explicit lambertian(const shared_ptr<texture>& a) : albedo(a) {}


	virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		vec3 scatter_direction = rec.normal + random_in_unit_sphere();

		// Catch degenerate scatter direction
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = ray(rec.pos, scatter_direction, r.get_time());
		attenuation = albedo->value(rec.pos, rec.u, rec.v);
		return true;
	}
	shared_ptr<texture> albedo;
};

class metal : public material
{
public:
	metal(const color& a, double f) : albedo(a) , fuzz(f){}

	bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		vec3 reflected = reflect(normalize(r.get_dir()), rec.normal);
		scattered = ray(rec.pos, reflected + fuzz * random_in_unit_sphere(), r.get_time());
		attenuation = albedo;
		return dot(scattered.get_dir(), rec.normal) > 0;
	}
	double fuzz;
	color albedo;
};

class dielectric : public material
{
public:
	dielectric(double ri) : ref_idx(ri){}

	
	virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = color(1.0, 1.0, 1.0);
		double etai_over_etat = rec.front_face ? (1.0 / ref_idx) : ref_idx;
		vec3 unit_direction = normalize(r.get_dir());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		if(etai_over_etat * sin_theta > 1.0)
		{
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.pos, reflected, r.get_time());
			return true;
		}
		double reflect_prob = schlick(cos_theta, etai_over_etat);
		if(random_double() < reflect_prob)
		{
			vec3 reflected = reflect(unit_direction, rec.normal);
			scattered = ray(rec.pos, reflected, r.get_time());
			return true;
		}
		vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
		scattered = ray(rec.pos, refracted, r.get_time());
		return true;
	}
	double ref_idx;
};