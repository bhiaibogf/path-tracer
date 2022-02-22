//
// Created by bhiaibogf on 2022/1/23.
//

#ifndef PATH_TRACER_MATERIAL_H
#define PATH_TRACER_MATERIAL_H


#include "../utils/global.h"

class Material {
public:
    virtual bool IsEmitter() const = 0;

    virtual global::Color emission() const = 0;

    virtual global::Color
    Eval(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const = 0;

    virtual global::Vector Sample(const global::Vector &wo, const global::Vector &normal) const = 0;

    virtual float Pdf(const global::Vector &wo, const global::Vector &wi, const global::Vector &normal) const = 0;

protected:
    static global::Vector ToWorld(const global::Vector &local, const global::Vector &normal);

};


#endif //PATH_TRACER_MATERIAL_H
