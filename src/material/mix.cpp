//
// Created by bhiaibogf on 2022/2/26.
//

#include "mix.h"

Mix::Mix(Material *material_1, Material *material_2)
        : material_1_(material_1), material_2_(material_2) {
    float luminance_1 = global::Luminance(material_1->Albedo()), luminance_2 = global::Luminance(material_2->Albedo());
    ratio_ = luminance_1 / (luminance_1 + luminance_2);
}

global::Color Mix::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return ratio_ * material_1_->Eval(wo, wi, normal)
           + (1.f - ratio_) * material_2_->Eval(wo, wi, normal);
}

global::Color Mix::Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal,
                        const global::TexCoord &tex_coord) const {
    return ratio_ * material_1_->Eval(wo, wi, normal, tex_coord)
           + (1.f - ratio_) * material_2_->Eval(wo, wi, normal, tex_coord);
}

global::Vector Mix::Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const {
    global::Vector out;
    float pdf_1, pdf_2;
    if (generator::Rand() < ratio_) {
        out = material_1_->Sample(wo, normal, &pdf_1);
        *pdf = ratio_ * pdf_1 +
               (1 - ratio_) * material_2_->Pdf(wo, out, normal);
    } else {
        out = material_2_->Sample(wo, normal, &pdf_2);
        *pdf = ratio_ * material_1_->Pdf(wo, out, normal) +
               (1 - ratio_) * pdf_2;
    }
    return out;
}

float Mix::Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const {
    return ratio_ * material_1_->Pdf(wo, wi, normal)
           + (1 - ratio_) * material_2_->Pdf(wo, wi, normal);
}

std::ostream &operator<<(std::ostream &os, const Mix &mix) {
    os << "Mix:\n   " << *(Lambert *) mix.material_1_ << "\n   " << *(Phong *) mix.material_2_
       << "\n   ratio = " << mix.ratio_;
    return os;
}

global::Color Mix::Albedo() const {
    return material_1_->Albedo() * ratio_ + material_2_->Albedo() * (1.f - ratio_);
}
