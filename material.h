#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

class Material {
public:
    // BRDFから方向をサンプリングすると同時にBRDFの値を計算して返す
    // 戻り値: BREFの値
    // wi: サンプリングされた方向
    // pdf: サンプリングされた方向の密度関数
    virtual Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const = 0;
};

#endif;