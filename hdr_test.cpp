#include <iostream>
#include "hdr.h"
#include "bmp.h"

int main(int argc, char *argv[]) {
    Image<Vec3> img;

    LoadHDRImage(argv[1], img);

    std::cout << "width: " << img.width << std::endl;
    std::cout << "height: " << img.height << std::endl;

    double max = 0;
    double min = 1000;
    for(int i = 0; i < img.height * img.width; i++) {
        Vec3 c = img.lines[0][i];
        if (c.x > max) max = c.x;
        if (c.y > max) max = c.y;
        if (c.z > max) max = c.z;
        if (c.x < min) min = c.x;
        if (c.y < min) min = c.y;
        if (c.z < min) min = c.z;
    }

    for(int i = 0; i < img.height * img.width; i++) {
        Vec3 c = img.lines[0][i];
        c.x = c.x / max;
        c.y = c.y / max;
        c.z = c.z / max;
        img.lines[0][i] = c;
    }

    std::cout << "max" << max << std::endl;
    std::cout << "min" << min << std::endl;

    SaveBitmap("hdr_test.bmp", img);
    return 0;
}