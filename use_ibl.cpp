#define _USE_MATH_DEFINES
#include "vec3.h"
#include "sky.h"
#include "aggregate.h"
#include "camera.h"
#include "image.h"
#include "bmp.h"
#include <omp.h>

const int MAX_DEPTH = 100;
const double ROULETTE = 0.9;

Vec3 radiance(const Ray& init_ray, const Aggregate& aggregate, const Sky& sky) {
    Vec3 col;
    Vec3 throughput(1);
    Ray ray = init_ray;

    for(int depth = 0; depth < MAX_DEPTH; depth++) {
        Hit res;

        if(aggregate.intersect(ray, res)) {
            Vec3 n = res.hitNormal;
            Vec3 s, t;
            orthonormalBasis(n, s, t);
            // 出射方向をローカル座標系に変換
            Vec3 wo_local = worldToLocal(-ray.direction, s, n, t);

            auto hitMaterial = res.hitShape->material;
            auto hitLight = res.hitShape->light;

            col += throughput * hitLight->Le();

            // 方向のサンプリングとBRDFの評価
            Vec3 brdf;
            Vec3 wi_local;
            double pdf;
            brdf = hitMaterial->sample(wo_local, wi_local, pdf);
            double cos = cosTheta(wi_local);
            Vec3 wi = localToWorld(wi_local, s, n, t);

            throughput *= brdf * cos / pdf;

            ray = Ray(res.hitPos + 0.001 * res.hitNormal, wi);
        } else {
            // 空に飛んでいった場合
            col += throughput * sky.getRadiance(ray);
            break;
        }

        if(rnd() >= ROULETTE) break;
        else throughput /= ROULETTE;
    }
    return col;
}

int main() {
    const int N = 500;

    Image<Vec3> img(256, 256);
    PinholeCamera cam(Vec3(0, 0, 1), Vec3(0, 0, -1), 1);

    auto mat1 = std::make_shared<Diffuse>(Vec3(0.9));
    auto mat2 = std::make_shared<Diffuse>(Vec3(0.2, 0.2, 0.8));

    auto light1 = std::make_shared<Light>(Vec3(0));

    Aggregate aggregate;
    aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000, mat1, light1));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -3), 1, mat2, light1));

    IBL sky("venice_dawn_1_1k.hdr");

    #pragma omp parallel for schedule(dynamic, 1)
    for(int i = 0; i < img.width; i++) {
        for(int j = 0; j < img.height; j++) {
            for(int k = 0; k < N; k++) {
                double u = (2.0 * (i + rnd()) - img.width) / img.width;
                double v = (2.0 * (j + rnd()) - img.height) / img.height;

                Ray ray = cam.getRay(-u, -v);
                Vec3 col = radiance(ray, aggregate, sky);

                img.lines[j][i] += col;
            }

            if(omp_get_thread_num() == 0) {
                std::cout << double(j + i * img.height) / (img.width * img.height) * 100 << "\r" << std::flush;
            }
        }
    }

    divide(img, N);
    gamma_correction(img);

    SaveBitmap("use_ibl.bmp", img);

    return 0;
}