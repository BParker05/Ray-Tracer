#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittableList.h"
#include "material.h"
#include "sphere.h"

int main() {
    hittableList world;

    auto materialGround = make_shared<lambertian>(colour(0.8, 0.8, 0.0));
    auto materialCenter = make_shared<lambertian>(colour(0.1, 0.2, 0.5));
    auto materialLeft   = make_shared<metal>(colour(0.8, 0.8, 0.8), 0.3);
    auto materialRight  = make_shared<metal>(colour(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, materialGround));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, materialCenter));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, materialLeft));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, materialRight));

    camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxDepth = 50;

    cam.render(world);
}