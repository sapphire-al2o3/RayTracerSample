#include <memory>
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
    PinholeCamera cam(Vec3(0 , 0 ,0), Vec3(0, 0, -1), 1);

    Aggregate aggregate;
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -3), 1));
    // 床として使う球
    aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000));

    for(int i = 0; i < img.height; i++) {
        double v = (2.0 * i - img.height) / img.height;
        for(int j = 0; j < img.width; j++) {
            double u = (2.0 * j - img.width) / img.width;

            Ray ray = cam.getRay(-u, -v);

            Hit res;
            if(aggregate.intersect(ray, res)) {
                img.lines[i][j] = (res.hitNormal + 1) / 2;
            } else {
                img.lines[i][j] = Vec3(0);
            }
        }
    }

    SaveBitmap("aggregate_test.bmp", img);

    return 0;
}
