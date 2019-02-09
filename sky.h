#ifndef SKY_H
#define SKY_H
#include "vec3.h"
#include "ray.h"
#include "hdr.h"

class Sky {
public:
    // 受け取ったレイの来る空の放射輝度の値を返す
    // 戻り値: 空の放射輝度
    // ray: 放射輝度を取る方向のレイ
    virtual Vec3 getRadiance(const Ray& ray) const = 0;
};

class UniformSky : public Sky
{
public:
    Vec3 color;

    UniformSky(const Vec3& _color) : color(_color) {}

    Vec3 getRadiance(const Ray& ray) const {
        return color;
    }
};

class SimpleSky : public Sky {
public:
    SimpleSky() {}

    Vec3 getRadiance(const Ray& ray) const {
        double t = (ray.direction.y + 1) / 2;
        return (1 - t) * Vec3(1) + t * Vec3(1, 0.5, 0.2);
    }
};

class IBL : public Sky {
public:
    int width;
    int height;
    Image<Vec3> image;

    
    IBL(const std::string& filename) {
        LoadHDRImage(filename.c_str(), image);
    }

    Vec3 getRadiance(const Ray& ray) const {
        double theta = std::acos(ray.direction.y);
        double phi = std::atan2(ray.direction.z, ray.direction.x);
        if(phi < 0) phi += 2 * M_PI;

        int i = phi / (2 * M_PI) * width;
        int j = theta / M_PI * height;

        return image.lines[j][i];
    }
};

#endif