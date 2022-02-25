//
// Created by bhiaibogf on 2022/2/25.
//

#include "../scene/object.h"

Primitive::Primitive(Object *object) : object_(object) {}

Material *Primitive::material() const {
    return object_->material();
}

