#include "utils.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "camera.h"
#include "hittable_list.h"
#include <iostream>
#include <chrono>



color ray_color(const ray& r, const hittable& world) {
    hit_record hit_rec;
    bool was_hit = world.hit(r, 0, infinity, hit_rec);
    if (was_hit) {
        return 0.5*color(
            hit_rec.normal.x()+1,
            hit_rec.normal.y()+1,
            hit_rec.normal.z()+1)
        ;
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}



int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    camera cam;


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

                cumulative_pixel_color += ray_color(cam.get_ray(u, v), world);
            }


            write_color(std::cout, cumulative_pixel_color, samples_per_pixel);

        }
    }
    std::chrono::steady_clock::time_point end_t = std::chrono::steady_clock::now();
    std::cerr << "all done in ";
    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(end_t - begin_t).count() << "ms\n";
}