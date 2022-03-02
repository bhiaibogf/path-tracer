//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_MATERIAL_H
#define PATH_TRACER_MATERIAL_H


#include "../utils/generator.h"
#include "../utils/global.h"

class Material {
public:
    Material();

    virtual ~Material() = default;

    enum MaterialType {
        kLambert, kPhong, kRefraction, kMix
    };

    virtual MaterialType Type() const = 0;

    virtual bool HasEmitter() const;

    void SetEmission(const global::Color &emission) { emission_ = emission; }

    auto emission() const { return emission_; }

    virtual global::Color Albedo() const { return global::kBlack; }

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

    static global::Vector ToWorld(const global::Vector &local, const global::Vector &normal);

};


#endif //PATH_TRACER_MATERIAL_H
