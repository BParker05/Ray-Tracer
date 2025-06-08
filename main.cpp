#include "utility.h"

#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"

int main() {
    hittableList world;

    auto groundMaterial = make_shared<lambertian>(colour(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMat = randomDouble();
            point3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphereMaterial;

                if (chooseMat < 0.8) {
                    // diffuse
                    auto albedo = colour::random() * colour::random();
                    sphereMaterial = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                } else if (chooseMat < 0.95) {
                    // metal
                    auto albedo = colour::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphereMaterial = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspectRatio      = 16.0 / 9.0;
    cam.imageWidth       = 1200;
    cam.samplesPerPixel = 5;
    cam.maxDepth         = 50;
    cam.antialiasing = false;

    cam.vfov     = 20;
    cam.lookFrom = point3(13,2,3);
    cam.lookAt   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocusAngle = 0.6;
    cam.focusDistance    = 10.0;

    cam.render(world);
}