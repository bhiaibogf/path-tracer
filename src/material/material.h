//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_MATERIAL_H
#define PATH_TRACER_MATERIAL_H

#include "parameter.h"

#include "../utils/generator.h"
#include "../utils/global.h"

class Material {
public:
    Material();

    virtual ~Material() = default;

    enum MaterialType {
        kLambert, kPhong, kRefraction, kGgx, kMix
    };

    virtual MaterialType Type() const = 0;

    virtual bool HasEmitter() const;

    void SetEmission(const global::Color &emission) { emission_ = emission; }

    auto emission() const { return emission_; }

    virtual global::Color Albedo() const;

    virtual global::Color Albedo(const global::TexCoord &tex_coord) const;

    virtual global::Color
    Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const = 0;

    virtual global::Color Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal,
                               const global::TexCoord &tex_coord) const {
        return Eval(wo, wi, normal);
    }

    virtual global::Vector Sample(const global::Vector &wo, const global::Vector &normal, float *pdf) const = 0;

    virtual float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Material &material);

protected:
    global::Color emission_;

};


#endif //PATH_TRACER_MATERIAL_H
