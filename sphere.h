#ifndef SPHERE_H
#define SPHERE_H
#include <cmath>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "light.h"
#include "shape.h"

class Sphere : public Shape
{
public:
    Vec3 center;
    double radius;

    // Sphere(const Vec3& _center, double _radius) : center(_center), radius(_radius) {}
    Sphere(const Vec3& _center, double _radius, const std::shared_ptr<Material>& _material, const std::shared_ptr<Light>& _light) : center(_center), radius(_radius), Shape(_material, _light) {}

    bool intersect(const Ray& ray, Hit& res) const
    {
        double b = dot(ray.direction, ray.origin - center);
        double c = (ray.origin - center).length2() - radius * radius;

        double d = b * b - c;

        double t1 = -b - std::sqrt(d);
        double t2 = -b + std::sqrt(d);

        if(t1 > 10000 | t2 < 0.001) return false;
        double t = t1;
        if(t1 < 0.001)
        {
            t = t2;
            if(t2 > 10000)
            {
                return false;
            }
        }

        res.t = t;
        res.hitPos = ray(t);
        res.hitNormal = normalize(res.hitPos - center);
        res.hitShape = this;

        return true;
    }
};

#endif
