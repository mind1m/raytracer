#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};


// here we reflect the light in the sphere along the normale
// diffuse
class lambertian : public material {
    public:
        color albedo;

        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            attenuation = albedo;
            vec3 scattered_point = rec.p;
            vec3 scattered_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scattered_direction.near_zero())
                scattered_direction = rec.normal;

            scattered = ray(scattered_point, scattered_direction);
            return true;
        }
};


class metal : public material {
    public:
        color albedo;
        double fuzz;

        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            attenuation = albedo;
            vec3 scattered_point = rec.p;
            vec3 scattered_direction = reflect(r_in.direction(), rec.normal);

            // add a bit of fuzz so it is not glass-like
            scattered_direction += fuzz * random_in_unit_sphere();

            scattered = ray(scattered_point, scattered_direction);
            return true;
        }

};

#endif