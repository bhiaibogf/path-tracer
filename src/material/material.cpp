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
    // t = global::Vector(0, 0, 1);
    // b = normal.cross(t);
    if (std::abs(normal.x()) > std::abs(normal.y())) {
        float inv_len = 1.f / std::sqrt(normal.x() * normal.x() + normal.z() * normal.z());
        b = global::Vector(normal.z() * inv_len, 0.f, -normal.x() * inv_len);
    } else {
        float inv_len = 1.f / std::sqrt(normal.y() * normal.y() + normal.z() * normal.z());
        b = global::Vector(0.f, normal.z() * inv_len, -normal.y() * inv_len);
    }
    t = b.cross(normal);
    return (local.x() * t + local.y() * b + local.z() * normal).normalized();
}

std::ostream &operator<<(std::ostream &os, const Material &material) {
    using namespace global;
    os << "\n\temission = " << material.emission_;
    return os;
}

global::Color Material::Albedo() const {
    return global::kBlack;
}

global::Color Material::Albedo(const global::TexCoord &tex_coord) const {
    return Albedo();
}
