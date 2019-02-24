#ifndef PLANE_H
#define PLANE_H
#include <cmath>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "light.h"
#include "shape.h"

class Plane : public Shape
{
public:
    Vec3 center;
    Vec3 normal;

    Plane(const Vec3& _center, const Vec3& _normal) : center(_center), normal(normalize(_normal)) {}
    Plane(const Vec3& _center, const Vec3& _normal, const std::shared_ptr<Material>& _material, const std::shared_ptr<Light>& _light) : center(_center), normal(normalize(_normal)), Shape(_material, _light) {}

    bool intersect(const Ray& ray, Hit& res) const
    {
        double c = dot(normal, ray.direction);

        if(c > -0.001) return false;

        double h = dot(center, normal);
        double t = (h - dot(normal, ray.origin)) / c;
        
        if(t > 10000 || t < 0.001) return false;

        res.t = t;
        res.hitNormal = normal;
        res.hitPos = ray(t);
        res.hitShape = this;

        return true;
    }
};

#endif
