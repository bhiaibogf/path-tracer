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
