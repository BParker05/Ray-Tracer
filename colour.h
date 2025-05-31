#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.h"
#include "rtweekend.h"

using colour = vec3;

void write_colour(std::ostream& out, const colour& pixelColour) {
    auto r = pixelColour.x();
    auto g = pixelColour.y();
    auto b = pixelColour.z();

    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif 