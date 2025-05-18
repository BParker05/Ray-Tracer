#include <iostream>
#include <fstream>

#include "colour.h"
#include "vec3.h"

int main() {

    // Image

    int imageWidth = 512;
    int imageHeight = 512;

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
            auto pixelColour = colour(double(i) / (imageWidth - 1), double(j) / (imageHeight - 1), 0.25);
            write_colour(outFile, pixelColour);
        }
    }
    std::clog << "\rDone.                 \n";
}