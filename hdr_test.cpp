#include <iostream>
#include "hdr.h"
#include "bmp.h"

int main(int argc, char *argv[]) {
    Image<Vec3> img;

    LoadHDRImage(argv[1], img);

    std::cout << "width: " << img.width << std::endl;
    std::cout << "height: " << img.height << std::endl;

    // double m = 0;
    // for(int i = 0; i < img.height * img.width; i++) {
    //     Vec3 c = img.lines[0][i];
    //     if (c.x > m) m = c.x;
    //     if (c.y > m) m = c.y;
    //     if (c.z > m) m = c.z;
    // }

    // for(int i = 0; i < img.height * img.width; i++) {
    //     Vec3 c = img.lines[0][i];
    //     c.x = 1;
    //     c.y = 1;
    //     c.z = 1;
    //     img.lines[0][i] = c;
    // }

    // SaveBitmap("hdr_test.bmp", img);
    return 0;
}