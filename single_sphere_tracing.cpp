#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "camera.h"
#include "sphere.h"
#include "image.h"
#include "bmp.h"

int main() {
    Image<Vec3> img(256, 256);

    PinholeCamera cam(Vec3(0, 0, 0), Vec3(0, 0, -1), 1);

    Sphere sphere(Vec3(0, 0, -3), 1);

    for(int i = 0; i < img.height; i++) {
        double v = (2.0 * i - img.height) / img.height;
        for(int j = 0; j < img.width; j++) {
            double u = (2.0 * j - img.width) / img.width;

            Ray ray = cam.getRay(-u, -v);

            // 球と当たった場合は法線を色にして書き込む
            Vec3 col;
            Hit res;
            if(sphere.intersect(ray, res)) {
                img.lines[i][j] = (res.hitNormal + 1) / 2;
            } else {
                img.lines[i][j] = Vec3(0, 0, 0);
            }
        }
    }

    SaveBitmap("single_sphere_tracing.bmp", img);

    return 0;
}