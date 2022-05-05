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

void Material::Prepare(const global::TexCoord &tex_coord) {}

global::Color Material::Albedo(const global::TexCoord &tex_coord) {
    Prepare(tex_coord);
    return Albedo();
}

global::Color Material::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal,
                             const global::TexCoord &tex_coord) {
    Prepare(tex_coord);
    return Eval(wo, wi, normal);
}

global::Vector Material::Sample(const global::Vector &wo, const global::Vector &normal, float *pdf,
                                const global::TexCoord &tex_coord) {
    Prepare(tex_coord);
    return Sample(wo, normal, pdf);
}

float Material::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal,
                    const global::TexCoord &tex_coord) {
    Prepare(tex_coord);
    return Pdf(wo, wi, normal);
}

std::ostream &operator<<(std::ostream &os, const Material &material) {
    using namespace global;
    os << "\n\temission = " << material.emission_;
    return os;
}

global::Color Material::Albedo() const {
    return global::kBlack;
}
