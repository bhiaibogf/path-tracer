//
// Created by bhiaibogf on 2022/2/21.
//

#include "object.h"

Object::Object(std::string name) : primitive_(nullptr), material_(nullptr), name_(std::move(name)) {}

void Object::SetPrimitive(Primitive *primitive) {
    switch (primitive->Type()) {
        case Primitive::kMesh:
            std::cout << "  " << *(Mesh *) primitive << std::endl;
            break;
        case Primitive::kSphere:
            std::cout << "  " << *(Sphere *) primitive << std::endl;
            break;
        case Primitive::kTriangle:
            std::cout << "  " << *(Triangle *) primitive << std::endl;
            break;
    }
    primitive_ = primitive;
}

void Object::SetMaterial(Material *material) {
    switch (material->Type()) {
        case Material::kLambert:
            std::cout << "  " << *(Lambert *) material << std::endl;
            break;
        case Material::kPhong:
            std::cout << "  " << *(Phong *) material << std::endl;
            break;
        case Material::kRefraction:
            std::cout << "  " << *(Refraction *) material << std::endl;
            break;
        case Material::kMix:
            std::cout << "  " << *(Mix *) material << std::endl;
            break;
    }
    material_ = material;
}

void Object::InsertTo(std::vector<const Primitive *> *primitives) const {
    primitive_->InsertTo(primitives);
}

bool Object::Intersect(Ray *ray, Intersection *intersection) const {
    if (primitive_->Intersect(ray, intersection)) {
        intersection->material = material_;
        return true;
    }
    return false;
}

void Object::Sample(Intersection *intersection, float *pdf) const {
    return primitive_->Sample(intersection, pdf);
}
