#ifndef HIT_H
#define HIT_H
#include "vec3.h"

class Shape;

class Hit
{
public:
    double t;           // 衝突距離
    Vec3 hitPos;        // 衝突位置
    Vec3 hitNormal;     // 衝突法線
    const Shape* hitShape;    // 衝突物体

    Hit() {
        t = 10000;
    }
};

#endif
