#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <fstream>

class camera {
    public:
        double aspectRatio = 1.0;
        int imageWidth = 512;
        int samplesPerPixel = 10;
        int maxDepth = 10;

        void render(const hittable& world){
            initialize();

            std::string outFileName {"image.ppm"};

            std::ofstream outFile(outFileName);

            outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

            for (int j = 0; j < imageHeight; j++) {
                std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
                for (int i = 0; i < imageWidth; i++) {
                    colour pixelColour(0, 0, 0);
                    for (int sample = 0; sample < samplesPerPixel; sample++) {
                        ray r = getRay(i,j);
                        pixelColour += rayColour(r, maxDepth, world);
                    }
                    write_colour(outFile, pixelSampleScale * pixelColour);
                }
            }
            std::clog << "\rDone.                 \n";
        }

    private:
        int imageHeight;
        double pixelSampleScale;
        point3 center;
        point3 pixel100Loc;
        vec3 pixelDeltaU;
        vec3 pixelDeltaV;

        void initialize(){
            imageHeight = int(imageWidth / aspectRatio);
            imageHeight = (imageHeight < 1) ? 1 : imageHeight;

            pixelSampleScale = 1.0 / samplesPerPixel;

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

        ray getRay(int i, int j) const {
            auto offset = sampleSquare();
            auto pixelSample = pixel100Loc + ((i + offset.x()) * pixelDeltaU) + ((j + offset.y()) * pixelDeltaV);

            auto rayOrigin = center;
            auto rayDirection = pixelSample - rayOrigin;

            return ray(rayOrigin, rayDirection);
        }

        vec3 sampleSquare() const {
            return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
        }

        colour rayColour(const ray& r, int depth, const hittable& world) {
            if (depth <= 0) {
                return colour(0,0,0);
            }

            hitRecord rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            colour attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * rayColour(scattered, depth-1, world);
            }
            return colour(0,0,0);
            }

            vec3 unitDirection = unitVector(r.direction());
            auto a = 0.5 * (unitDirection.y() + 1.0);
            return (1.0-a)*colour(1.0, 1.0, 1.0) + a*colour(0.5, 0.7, 1.0);
}
};

#endif