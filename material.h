#ifndef MATERIAL_H
#define MATERIAL_H

#define _USE_MATH_DEFINES
#include <cmath>
#include "vec3.h"
#include "random.h"

class Material {
public:
    // BRDFから方向をサンプリングすると同時にBRDFの値を計算して返す
    // 戻り値: BREFの値
    // wi: サンプリングされた方向
    // pdf: サンプリングされた方向の密度関数
    virtual Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const = 0;
};

class Diffuse : public Material {
public:
    // 反射率
    Vec3 rho;

    Diffuse(const Vec3& _rho) : rho(_rho) {}

    Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
        // 一様乱数
        double u = rnd();
        double v = rnd();
        
        // (theta, phi)の計算
        double theta = 0.5 * std::acos(1 - 2 * u);
        double phi = 2 * M_PI * v;

        // (x, y, z)の計算
        double x = std::cos(phi) * std::sin(theta);
        double y = std::cos(theta);
        double z = std::sin(phi) * std::sin(theta);

        // サンプリングされた方向
        wi = Vec3(x, y, z);

        // 確率密度関数の値
        pdf = std::cos(theta) / M_PI;

        return rho / M_PI;
    }
};

Vec3 reflect(const Vec3& v, const Vec3& n) {
    return -v + 2 * dot(v, n) * n;
}

// ローカル座標系のベクトルからcosThetaを計算する
// 戻り値: コサイン
// v: ローカル座標系のベクトル
double cosTheta(const Vec3& v) {
    return v.y;
}

class Mirror : public Material {
public:
    Mirror() {}

    Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
        // サンプリング方向は反射ベクトルのみ
        wi = reflect(wo, Vec3(0, 1, 0));

        // デルタ関数の部分は打ち消される
        pdf = 1.0;
        return 1 / cosTheta(wi) * Vec3(1);
    }
};

#endif;