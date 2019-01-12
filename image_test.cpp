#include "vec3.h"
#include "image.h"
#include "bmp.h"

int main() {
    Image<Vec3> img(512, 512);
    for(int i = 0; i < img.width; i++) {
        for(int j = 0; j < img.height; j++) {
            img.lines[j][i] = Vec3(i / (double)img.width, j / (double)img.height, 1.0);
        }
    }
    // ppm_output(img, "image_test.ppm");
    SaveBitmap("image_test.bmp", img);

    return 0;
}