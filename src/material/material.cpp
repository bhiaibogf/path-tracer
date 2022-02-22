//
// Created by bhiaibogf on 2022/1/23.
//

#include "material.h"

global::Vector Material::ToWorld(const global::Vector &local, const global::Vector &normal) {
    global::Vector t, b;
    if (std::fabs(normal.x()) > std::fabs(normal.y())) {
        float invLen = 1.f / std::sqrt(normal.x() * normal.x() + normal.z() * normal.z());
        b = global::Vector(normal.z() * invLen, 0.f, -normal.x() * invLen);
    } else {
        float invLen = 1.0f / std::sqrt(normal.y() * normal.y() + normal.z() * normal.z());
        b = global::Vector(0.f, normal.z() * invLen, -normal.y() * invLen);
    }
    t = b.cross(normal);
    return local.x() * t + local.y() * b + local.z() * normal;
}
