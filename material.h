#pragma once

#include "geometry.h"
#include "ray.h"
#include "hittable.h"
#include "texture.h"

struct hit_record;

//材质抽象类
class material
{
public:
	virtual color emitted(const point3& p, double u, double v) const
	{
		return color(0, 0, 0);
	}
	virtual bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

//近似兰伯特材质
class lambertian : public material
{
public:
	explicit lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
	explicit lambertian(const shared_ptr<texture>& a) : albedo(a) {}

	bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override
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

//金属材质
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

//电解质材质：玻璃等折射物体
class dielectric : public material
{
public:
	dielectric(double ri) : ref_idx(ri){}

	
	bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override
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

//光源材质
class diffuse_light : public material
{
public:
	diffuse_light(std::shared_ptr<texture> a) : emission(a) {}
	diffuse_light(color c) : emission(make_shared<solid_color>(c)) {}

	bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		return false;
	}

	color emitted(const point3& p, double u, double v) const override
	{
		return emission->value(p, u, v);
	}

	std::shared_ptr<texture> emission;
};

//volume isotropic 各向同性体积材质
class isotropic : public material
{
public:
	isotropic(color c) : albedo(make_shared<solid_color>(c)) {}

	isotropic(shared_ptr<texture> a) : albedo(std::move(a)) {}

	bool scatter(const ray& r, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		scattered = ray(rec.pos, random_in_unit_sphere(), r.get_time());
		attenuation = albedo->value(rec.pos, rec.u, rec.v);
		return true;
	}

	shared_ptr<texture> albedo;
};