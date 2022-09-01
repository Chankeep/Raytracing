#pragma once

#include <utility>

#include "geometry.h"

class texture {
public:
    virtual color value(const vec3& p, double u, double v) const = 0;
};

class solid_color : public texture {
public:
    solid_color() = default;
    solid_color(color c) : color_value(c) {}

    solid_color(double red, double green, double blue)
        : solid_color(color(red, green, blue)) {}

    virtual color value(const vec3& p, double u, double v) const override {
        return color_value;
    }

private:
    color color_value;
};

class checker_texture : public texture {
public:
    checker_texture() = default;

    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
        : odd(std::move(_odd)), even(std::move(_even)) {}

    checker_texture(color c1, color c2)
        : odd(make_shared<solid_color>(c2)), even(make_shared<solid_color>(c1)) {}

    color value(const point3& p, double u, double v) const override {
        auto sines = sin(1000 * p.x()) * sin(1000 * p.y()) * sin(1000 * p.z());
        if (sines < 0)
            return odd->value(p, u, v);
        else
            return even->value(p, u, v);
    }

public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;
};