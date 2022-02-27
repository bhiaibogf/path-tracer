//
// Created by bhiaibogf on 2022/2/26.
//

#ifndef PATH_TRACER_MIX_H
#define PATH_TRACER_MIX_H


#include "lambert.h"
#include "phong.h"
#include "refraction.h"

class Mix : public Material {
public:
    Mix(Material *material_1, Material *material_2, float ratio = 0.5f);

    ~Mix() override = default;

    MaterialType Type() const override { return kMix; }

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal,
                       const global::TexCoord &tex_coord) const override;

    global::Vector Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const override;

    float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    friend std::ostream &operator<<(std::ostream &os, const Mix &mix);

private:
    Material *material_1_;
    Material *material_2_;
    float ratio_;

};


#endif //PATH_TRACER_MIX_H
