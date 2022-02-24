//
// Created by bhiaibogf on 2022/1/23.
//

#include "material.h"

Material::Material() {
    emission_ = global::kBlack;
}

bool Material::HasEmitter() const {
    return emission_ != global::kBlack;
}

global::Vector Material::ToWorld(const global::Vector &local, const global::Vector &normal) {
    global::Vector t, b;
    if (std::fabs(normal.x()) > std::fabs(normal.y())) {
        float len = 1.f / std::sqrt(normal.x() * normal.x() + normal.z() * normal.z());
        b = global::Vector(normal.z() / len, 0.f, -normal.x() / len);
    } else {
        float len = 1.0f / std::sqrt(normal.y() * normal.y() + normal.z() * normal.z());
        b = global::Vector(0.f, normal.z() / len, -normal.y() / len);
    }
    t = b.cross(normal);
    return local.x() * t + local.y() * b + local.z() * normal;
}

std::ostream &operator<<(std::ostream &os, const Material &material) {
    using namespace global;
    os << "\n\temission = " << material.emission_;
    return os;
}