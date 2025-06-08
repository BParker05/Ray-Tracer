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
        bool antialiasing;

        double vfov = 90.0;
        point3 lookFrom = point3(0,0,0);
        point3 lookAt = point3(0,0,-1);
        vec3 vup = vec3(0,1,0);

        double defocusAngle = 0;
        double focusDistance = 10;

        void render(const hittable& world){
            initialize();

            std::string outFileName {"image.ppm"};

            std::ofstream outFile(outFileName);

            outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

            for (int j = 0; j < imageHeight; j++) {
                std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
                for (int i = 0; i < imageWidth; i++) {
                    if (antialiasing) {
                        drawWithAntialiasing(outFile, i, j, world);
                    } else {
                        drawWithoutAntialiasing(outFile, i, j, world);
                    }
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
        vec3 u, v, w;
        vec3 defocusDiskU;
        vec3 defocusDiskV;

        void drawWithoutAntialiasing(std::ostream& out, int i, int j, const hittable& world){
            auto pixelCenter = pixel100Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
                auto rayDirection = pixelCenter - center;
                ray r(center, rayDirection);

                colour pixelColour = rayColour(r, maxDepth, world);
                writeColour(out, pixelColour);
        }

        void drawWithAntialiasing(std::ostream& out, int i, int j, const hittable& world){
            colour pixelColour(0, 0, 0);
            for (int sample = 0; sample < samplesPerPixel; sample++) {
                ray r = getRay(i,j);
                pixelColour += rayColour(r, maxDepth, world);
            }
            writeColour(out, pixelSampleScale * pixelColour);
        }

        void initialize(){
            imageHeight = int(imageWidth / aspectRatio);
            imageHeight = (imageHeight < 1) ? 1 : imageHeight;

            pixelSampleScale = 1.0 / samplesPerPixel;

            center = lookFrom;

            auto theta = degreesToRadians(vfov);
            auto h = std::tan(theta/2);
            auto viewportHeight = 2 * h * focusDistance;
            auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

            w = unitVector(lookFrom - lookAt);
            u = unitVector(cross(vup, w));
            v = cross(w, u);

            auto viewportU = viewportWidth * u;
            auto viewportV = viewportHeight * -v;

            pixelDeltaU = viewportU / imageWidth;
            pixelDeltaV = viewportV / imageHeight;

            auto viewportUpperLeft = center - (focusDistance * w) - viewportU/2 - viewportV/2;
            pixel100Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

            auto defocusRadius = focusDistance * std::tan(degreesToRadians(defocusAngle / 2));
            defocusDiskU = u * defocusRadius;
            defocusDiskV = v * defocusRadius;            
        }

        ray getRay(int i, int j) const {
            auto offset = sampleSquare();
            auto pixelSample = pixel100Loc + ((i + offset.x()) * pixelDeltaU) + ((j + offset.y()) * pixelDeltaV);

            auto rayOrigin = (defocusAngle <= 0) ? center : defocusDiskSample();
            auto rayDirection = pixelSample - rayOrigin;

            return ray(rayOrigin, rayDirection);
        }

        vec3 sampleSquare() const {
            return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
        }
        
        point3 defocusDiskSample() const {
            auto p = randomInUnitDisk();
            return center + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
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