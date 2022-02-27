//
// Created by bhiaibogf on 2022/2/23.
//

#ifndef PATH_TRACER_REFRACTION_H
#define PATH_TRACER_REFRACTION_H


#include "material.h"

class Refraction : public Material {
public:
    Refraction(float n_i_);

    MaterialType Type() const override { return kRefraction; }

    global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    global::Vector Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const override;

    float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    friend std::ostream &operator<<(std::ostream &os, const Refraction &refraction);

private:
    static const float n_s;
    float ior_;

};


#endif //PATH_TRACER_REFRACTION_H
