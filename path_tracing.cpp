#define _USE_MATH_DEFINES
#include <memory>
#include <omp.h>
#include "camera.h"
#include "light.h"
#include "sphere.h"
#include "aggregate.h"
#include "image.h"
#include "bmp.h"

// 最大反射回数
const int MAX_DEPTH = 500;
// ロシアンルーレットの確率
const double ROULETTE = 0.9;

Vec3 radiance(const Ray& init_ray, const Aggregate& aggregate)
{
    // 最終的に返す色
    Vec3 col;
    // スループット
    Vec3 throughput(1);
    // 更新していくレイ
    Ray ray = init_ray;

    // 級数の評価
    for(int depth = 0; depth < MAX_DEPTH; depth++) {
        Hit res;
        // レイとシーンが衝突した場合
        if(aggregate.intersect(ray, res)) {
            Vec3 n = res.hitNormal;
            // ローカル座標の構築
            Vec3 s, t;
            orthonormalBasis(n, s, t);
            // 出射方向をローカル座標系に変換
            Vec3 wo_local = worldToLocal(-ray.direction, s, n, t);
        
            // マテリアルと光源
            auto hitMaterial = res.hitSphere->material;
            auto hitLight = res.hitSphere->light;

            // Leの加算
            col += throughput * hitLight->Le();

            // 方向のサンプリングとBRDFの評価
            Vec3 brdf;
            Vec3 wi_local;
            double pdf;
            brdf = hitMaterial->sample(wo_local, wi_local, pdf);
            double cos = cosTheta(wi_local);
            Vec3 wi = localToWorld(wi_local, s, n, t);

            // スループットの更新
            throughput *= brdf * cos / pdf;

            // 次のレイを生成
            ray = Ray(res.hitPos + 0.001 * res.hitNormal, wi);
        } else {
            // 空に飛んでいった場合
            col += throughput * Vec3(1);
            break;
        }

        // ロシアンルーレット
        if(rnd() >= ROULETTE) {
            break;
        } else {
            throughput /= ROULETTE;
        }
    }
    return col;
}

int main() {
    // サンプル数
    const int N = 100;

    Image<Vec3> img(256, 256);
    PinholeCamera cam(Vec3(0, 0, 1), Vec3(0, 0, -1), 1);

    auto mat1 = std::make_shared<Diffuse>(Vec3(0.9));
    auto mat2 = std::make_shared<Diffuse>(Vec3(0.2, 0.2, 0.8));

    auto light1 = std::make_shared<Light>(Vec3(0));
    auto light2 = std::make_shared<Light>(Vec3(0));

    Aggregate aggregate;
    // 床
    aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000, mat1, light1));
    // 球
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -3), 1, mat2, light2));

    #pragma omp parallel for schedule(dynamic, 1)
    for(int i = 0; i < img.height; i++) {
        for(int j = 0; j < img.width; j++) {
            for(int k = 0; k < N; k++) {
                double u = (2.0 * (j + rnd()) - img.width) / img.width;
                double v = (2.0 * (i + rnd()) - img.height) / img.height;

                Ray ray = cam.getRay(-u, -v);
                // 放射輝度を計算
                Vec3 col = radiance(ray, aggregate);

                // サンプルを加算
                img.lines[i][j] += col;
            }

            if(omp_get_thread_num() == 0) {
                std::cout << double(j + i * img.height) / (img.width * img.height) * 100 << "\r" << std::flush;
            }
        }
    }

    // サンプリング数で割る
    divide(img, N);
    gamma_correction(img);

    SaveBitmap("path_tracing.bmp", img);

    return 0;
}