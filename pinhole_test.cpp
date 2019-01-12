#include "vec3.h"
#include "ray.h"
#include "image.h"
#include "bmp.h"
#include "camera.h"

int main() {
    Image<Vec3> img(512, 512);
    PinholeCamera cam(Vec3(0, 0, 0), Vec3(0, 0, -1), 1);

    for(int i = 0; i < img.height; i++) {
        double v = (2.0 * i - img.height) / img.height;
        for(int j = 0; j < img.width; j++) {
            double u = (2.0 * j - img.width) / img.width;

            Ray ray = cam.getRay(-u, -v);
            Vec3 col = (ray.direction + 1.0) / 2.0;
            img.lines[i][j] = col;
        }
    }

    SaveBitmap("pinhole_test.bmp", img);

    return 0;
}
