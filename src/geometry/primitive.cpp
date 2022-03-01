//
// Created by bhiaibogf on 2022/2/25.
//

#include "../scene/object.h"

Primitive::Primitive(Object *object) : object_(object), area_(0.f) {}

Material *Primitive::material() const {
    return object_->material();
}

float Primitive::Weight() const {
    return material()->emission().squaredNorm();
}

float Primitive::AreaWeighted() const {
    return area_ * Weight();
}
