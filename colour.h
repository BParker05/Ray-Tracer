#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.h"
#include "interval.h"

using colour = vec3;

inline double linearToGamma(double linearComponent) {
    if (linearComponent > 0){
        return std::sqrt(linearComponent);
    }

    return 0.0;
}

void write_colour(std::ostream& out, const colour& pixelColour) {
    auto r = pixelColour.x();
    auto g = pixelColour.y();
    auto b = pixelColour.z();

    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif 