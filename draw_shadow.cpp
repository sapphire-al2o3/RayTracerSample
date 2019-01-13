#include <memory>
#include <algorithm>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "sphere.h"
#include "camera.h"
#include "image.h"
#include "aggregate.h"
#include "bmp.h"

int main() {
    Image<Vec3> img(256, 256);
    PinholeCamera cam(Vec3(0), Vec3(0, 0, -1), 1);

    Aggregate aggregate;
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -3), 1));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000));

    Vec3 sunDir = normalize(Vec3(1, 1, 1));

    for(int i = 0; i < img.height; i++) {
        double v = (2.0 * i - img.height) / img.height;
        for(int j = 0; j < img.width; j++) {
            double u = (2.0 * j - img.width) / img.width;

            Ray ray = cam.getRay(-u, -v);

            Vec3 col;
            Hit res;
            if(aggregate.intersect(ray, res)) {
                Ray shadowRay(res.hitPos + 0.001 * res.hitNormal, sunDir);
                Hit res_temp;

                if(!aggregate.intersect(shadowRay, res_temp)) {
                    col = std::max(dot(res.hitNormal, sunDir), 0.0) * Vec3(1);
                }
            }

            img.lines[i][j] = col;
        }
    }

    SaveBitmap("draw_shadow.bmp", img);

    return 0;
}