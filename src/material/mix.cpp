//
// Created by bhiaibogf on 2022/2/26.
//

#include "mix.h"

Mix::Mix(Material *material_1, Material *material_2, float ratio)
        : material_1_(material_1), material_2_(material_2), ratio_(ratio) {}

global::Color Mix::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return material_1_->Eval(wo, wi, normal) + material_2_->Eval(wo, wi, normal);
}

global::Color Mix::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal,
                        const global::TexCoord &tex_coord) const {
    return material_1_->Eval(wo, wi, normal, tex_coord) + material_2_->Eval(wo, wi, normal, tex_coord);
}

global::Vector Mix::Sample(const global::Vector &wo, const global::Vector &normal) const {
    if (generator::Rand() < ratio_) {
        return material_1_->Sample(wo, normal);
    } else {
        return material_2_->Sample(wo, normal);
    }
}

float Mix::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return ratio_ * (material_1_->Pdf(wo, wi, normal) + (1 - ratio_) * material_2_->Pdf(wo, wi, normal));
}

std::ostream &operator<<(std::ostream &os, const Mix &mix) {
    os << "Mix:\n   " << *(Lambert *) mix.material_1_ << "\n   " << *(Phong *) mix.material_2_
       << "\n   ratio = " << mix.ratio_;
    return os;
}