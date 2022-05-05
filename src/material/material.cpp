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
    global::Color albedo;
#pragma omp critical
    {
        Prepare(tex_coord);
        albedo = Albedo();
    }
    return albedo;
}

global::Color Material::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal,
                             const global::TexCoord &tex_coord) {
    global::Color color;
#pragma omp critical
    {
        Prepare(tex_coord);
        color = Eval(wo, wi, normal);
    }
    return color;
}

global::Vector Material::Sample(const global::Vector &wo, const global::Vector &normal, float *pdf,
                                const global::TexCoord &tex_coord) {
    global::Color wi;
#pragma omp critical
    {
        Prepare(tex_coord);
        wi = Sample(wo, normal, pdf);
    }
    return wi;
}

float Material::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal,
                    const global::TexCoord &tex_coord) {
    float pdf;
#pragma omp critical
    {
        Prepare(tex_coord);
        pdf = Pdf(wo, wi, normal);
    }
    return pdf;
}

std::ostream &operator<<(std::ostream &os, const Material &material) {
    using namespace global;
    os << "\n\temission = " << material.emission_;
    return os;
}

global::Color Material::Albedo() const {
    return global::kBlack;
}
