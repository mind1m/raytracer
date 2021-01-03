#include "utils.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "hittable_list.h"
#include <iostream>
#include <chrono>



color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record hit_rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    bool was_hit = world.hit(r, 0.001, infinity, hit_rec);
    if (was_hit) {
        ray scattered;
        color attenuation;
        if (hit_rec.mat_ptr->scatter(r, hit_rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0,0,0);
    }

    // skybox
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    double small_radius = 0.2;
    // iterate and add a bunch of small spheres
    int bound = 10;  // how far do we draw small balls
    for (int a=-bound; a <= bound; a++) {
        for (int b=-bound; b <= bound; b++) {
            if ((a >= -4) & (a <= 4) & (b >= -1) & (b <= 1)) {
                // leave the center of the scene empty
                continue;
            }

            point3 center(a + 0.9*random_double(), small_radius, b + 0.9*random_double());
            double material_selector = random_double();
            shared_ptr<material> sphere_material;

            if (material_selector < 0.5) {
                auto albedo = color::random();
                sphere_material = make_shared<lambertian>(albedo);
            } else if (material_selector < 0.9) {
                auto albedo = color::random();
                auto fuzz = random_double();
                sphere_material = make_shared<metal>(albedo, fuzz);
            } else {
                auto refraction = 1 + random_double();
                sphere_material = make_shared<dielectric>(refraction);
            }
            world.add(make_shared<sphere>(center, small_radius, sphere_material));
        }
    }

    // add a few large spheres
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    return world;
}




int main() {

    // Image
    const bool quality_mode = false;
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = quality_mode ? 1440 : 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = quality_mode ? 500 : 5;
    const int max_depth = 50;

    // World
    hittable_list world = random_scene();

    // camera
    point3 lookfrom(12,2,6);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);


    std::chrono::steady_clock::time_point begin_t = std::chrono::steady_clock::now();


    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "Progress: " << image_height - j << "/" << image_height << "\n";
        for (int i = 0; i < image_width; ++i) {

            // sample each pixel samples_per_pixel times
            // randomly changing ray position
            color cumulative_pixel_color;
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);

                cumulative_pixel_color += ray_color(cam.get_ray(u, v), world, max_depth);
            }


            write_color(std::cout, cumulative_pixel_color, samples_per_pixel);

        }
    }
    std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();
    std::cerr << "all done in ";
    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(end_t - begin_t).count() << "ms\n";
}