#include <iostream>
#include <fstream>

#include "colour.h"
#include "ray.h"
#include "vec3.h"

bool hitSphere(const point3& center, double radius, const ray& r){
    vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc,oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
}

colour rayColour(const ray& r){
    if (hitSphere(point3(0,0,-1), 0.5, r))
        return colour(1, 0, 0);

    vec3 unitDirection = unitVector(r.direction());
    auto a = 0.5*(unitDirection.y() + 1.0);
    return (1.0-a)*colour(1.0,1.0,1.0) + a*colour(0.5,0.7,1.0);
}

int main() {

    // Image

    auto aspectRatio = 1/1;
    int imageWidth = 512;

    // Calculate the image height, and ensure that it's at least 1.
    int imageHeight = int(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;

    // Camera

    auto focalLength = 1.0;
    auto viewportHeight = 2.0;
    auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);
    auto cameraCenter = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewportU = vec3(viewportWidth,0,0);
    auto viewportV = vec3(0,-viewportHeight,0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixelDeltaU = viewportU / imageWidth;
    auto pixelDeltaV = viewportV / imageHeight;

    // Calculate the location of the upper left pixel.
    auto viewportUpperLeft = cameraCenter - vec3(0,0,focalLength) - viewportU/2 - viewportV/2;
    auto pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

    std::string outFileName {"image.ppm"};

    std::ofstream outFile(outFileName);

    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return 1; // Return error code
    }

    // Render

    outFile << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; j++) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            auto pixelCentre = pixel00Loc + (i * pixelDeltaU) + (j * pixelDeltaV);
            auto rayDirection = pixelCentre - cameraCenter;
            ray r(cameraCenter, rayDirection);

            colour pixelColour = rayColour(r);
            write_colour(outFile, pixelColour);
        }
    }
    std::clog << "\rDone.                 \n";
}