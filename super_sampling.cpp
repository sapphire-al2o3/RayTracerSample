#define _USE_MATH_DEFINES
#include <memory>
#include <random>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "sphere.h"
#include "camera.h"
#include "image.h"
#include "aggregate.h"
#include "bmp.h"

/*
std::random_device rnd_dev;
std::mt19937 mt(rnd_dev());
std::uniform_real_distribution<> dist(0, 1); // [0, 1]の一様乱数
inline double rnd() {
    return dist(mt);
}
*/

int main() {
    // スーパーサンプリング数
    const int N = 100;

    Image<Vec3> img(256, 256);
    PinholeCamera cam(Vec3(0), Vec3(0, 0, -1), 1);

    Aggregate aggregate;
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -3), 1));
    aggregate.add(std::make_shared<Sphere>(Vec3(1, 0, -2), 0.25));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000));

    Vec3 sunDir = normalize(Vec3(1, 1, 1));

    for(int k = 0; k < N; k++) {
        for(int i = 0; i < img.height; i++) {
            for(int j = 0; j < img.width; j++) {
                // 乱数でピクセル内の位置をずらす
                double u = (2.0 * (j + rnd()) - img.width) / img.width;
                double v = (2.0 * (i + rnd()) - img.height) / img.height;

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

                img.lines[i][j] = img.lines[i][j] + col;
            }
        }
    }

    divide(img, N);

    SaveBitmap("super_sampling.bmp", img);

    return 0;
}
