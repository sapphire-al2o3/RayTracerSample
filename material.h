#ifndef MATERIAL_H
#define MATERIAL_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
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

// ローカル座標系のベクトルからcosThetaの絶対値を計算する
// 戻り値: コサインの絶対値
// v: ローカル座標系のベクトル
double absCosTheta(const Vec3& v) {
    return std::abs(v.y);
}

bool refract(const Vec3& v, Vec3& r, const Vec3& n, double n1, double n2) {
    double cos = absCosTheta(v);
    double sin = std::sqrt(std::max(1 - cos * cos, 0.0));
    double alpha = n1 / n2 * sin;

    // 全反射
    if(alpha * alpha > 1.0) return false;

    // 屈折ベクトル
    r = n1 / n2 * (-v + dot(v, n) * n) - std::sqrt(1 - alpha * alpha) * n;
    return true;
}

double fresnel(const Vec3& v, const Vec3& n, double n1, double n2) {
    double f0 = std::pow((n1 - n2) / (n1 + n2), 2.0);
    double cos = absCosTheta(v);
    return f0 + (1 - f0) * std::pow(1 - cos, 5.0);
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

class Glass : public Material {
public:
    double ior; // 屈折率
    Glass(double _ior) : ior(_ior) {}

    Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
        // ガラスに入射しているか
        bool isEntering = cosTheta(wo) > 0;

        double n1;  // 入射側屈折率
        double n2;  // 出射側屈折率
        Vec3 normal;
        if(isEntering) {
            n1 = 1.0;
            n2 = ior;
            normal = Vec3(0, 1, 0);
        } else {
            n1 = ior;
            n2 = 1.0;
            normal = Vec3(0, -1, 0);
        }

        // フレネル反射率
        double fr = fresnel(wo, normal, n1, n2);

        // 反射
        if(rnd() < fr) {
            wi = reflect(wo, normal);
            pdf = fr;
            return fr / absCosTheta(wi) * Vec3(1);
        } else {
            if(refract(wo, wi, normal, n1, n2)) {
                pdf = 1 - fr;
                return std::pow(n1 / n2, 2.0) * (1 - fr) / absCosTheta(wi) * Vec3(1);
            } else {
                // 全反射
                wi = reflect(wo, normal);
                pdf = 1 - fr;
                return (1 - fr) / absCosTheta(wi) * Vec3(1);
            }
        }
    }
};

#endif;