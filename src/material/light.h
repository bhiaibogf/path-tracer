//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_LIGHT_H
#define PATH_TRACER_LIGHT_H


#include "material.h"

class Light : public Material {
public:
    Light(const global::Color &radiance);

    ~Light() = default;

    bool IsEmitter() const override;

    global::Color emission() const override;

    global::Color
    Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

    global::Vector Sample(const global::Vector &wo, const global::Vector &normal) const override;

    float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const override;

private:
    Eigen::Vector3f radiance_;

};


#endif //PATH_TRACER_LIGHT_H
