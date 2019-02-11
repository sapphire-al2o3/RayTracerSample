#ifndef AGGREGATE_H
#define AGGREGATE_H
#include <memory>
#include <vector>
#include "ray.h"
#include "hit.h"
#include "shape.h"
#include "sphere.h"

class Aggregate {
public:
    // 物体の配列
    std::vector<std::shared_ptr<Shape>> shapes;

    Aggregate() {};
    Aggregate(const std::vector<std::shared_ptr<Shape>>& _shapes) : shapes(_shapes) {}

    // 物体を追加する
    void add(const std::shared_ptr<Shape>& s) {
        shapes.push_back(s);
    }

    bool intersect(const Ray& ray, Hit& res) const {
        bool hit = false;
        for(auto s : shapes) {
            Hit res_temp;
            if(s->intersect(ray, res_temp)) {
                if(res_temp.t < res.t) {
                    hit = true;
                    res = res_temp;
                }
            }
        }
        return hit;
    }
};


#endif
