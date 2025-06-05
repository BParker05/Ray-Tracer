#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include <fstream>

class camera {
    public:
        double aspectRatio = 1.0;
        int imageWidth = 512;

        void render(const hittable& world){
            initialize();

            std::string outFileName {"image.ppm"};

            std::ofstream outFile(outFileName);

            outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

            for (int j = 0; j < imageHeight; j++) {
                std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
                for (int i = 0; i < imageWidth; i++) {
                    auto pixelCentre = pixel100Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
                    auto rayDirection = pixelCentre - center;
                    ray r(center, rayDirection);

                    colour pixelColour = rayColour(r, world);
                    write_colour(outFile, pixelColour);
                }
            }
            std::clog << "\rDone.                 \n";
        }

    private:
        int imageHeight;
        point3 center;
        point3 pixel100Loc;
        vec3 pixelDeltaU;
        vec3 pixelDeltaV;

        void initialize(){
            imageHeight = int(imageWidth / aspectRatio);
            imageHeight = (imageHeight < 1) ? 1 : imageHeight;

            center = point3(0, 0, 0);

            auto focalLength = 1.0;
            auto viewportHeight = 2.0;
            auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

            auto viewportU = vec3(viewportWidth, 0, 0);
            auto viewportV = vec3(0, -viewportHeight, 0);

            pixelDeltaU = viewportU / imageWidth;
            pixelDeltaV = viewportV / imageHeight;

            auto viewportUpperLeft = center - vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
            pixel100Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
        }

        colour rayColour(const ray& r, const hittable& world) {
            hitRecord rec;
            if (world.hit(r, interval(0, infinity), rec)) {
                return 0.5 * (rec.normal + colour(1, 1, 1));
            }

            vec3 unitDirection = unitVector(r.direction());
            auto a = 0.5 * (unitDirection.y() + 1.0);
            return (1.0-a)*colour(1.0, 1.0, 1.0) + a*colour(0.5, 0.7, 1.0);
}
};

#endif