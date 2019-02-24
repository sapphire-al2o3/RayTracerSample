#ifndef SHAPE_H
#define SHAPE_H
#include <cmath>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "light.h"

class Shape
{
public:
    // BRDF
    std::shared_ptr<Material> material;
    // Le
    std::shared_ptr<Light> light;

    Shape() {}
    Shape(const std::shared_ptr<Material>& _material, const std::shared_ptr<Light>& _light) : material(_material), light(_light) {}
    virtual bool intersect(const Ray& ray, Hit& res) const = 0;
};

#endif
