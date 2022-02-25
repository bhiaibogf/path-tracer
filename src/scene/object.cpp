//
// Created by bhiaibogf on 2022/2/21.
//

#include "object.h"

Object::Object() : mesh_(nullptr), material_(nullptr) {}

void Object::SetMesh(Mesh *mesh) {
    std::cout << "  " << *mesh << std::endl;
    mesh_ = mesh;
}

void Object::SetMaterial(Material *material) {
    switch (material_->Type()) {
        case Material::kLambert:
            std::cout << "  " << *(Lambert *) material_ << std::endl;
            break;
        case Material::kPhong:
            std::cout << "  " << *(Phong *) material_ << std::endl;
            break;
        case Material::kRefraction:
            std::cout << "  " << *(Refraction *) material_ << std::endl;
            break;
    }
    material_ = material;
}

void Object::InsertTo(std::vector<Primitive *> *primitives) const { mesh_->InsertTo(primitives); }

bool Object::Intersect(Ray *ray, Intersection *intersection) const {
    if (mesh_->Intersect(ray, intersection)) {
        intersection->material = material_;
        return true;
    }
    return false;
}

void Object::Sample(Intersection *intersection, float *pdf) const {
    return mesh_->Sample(intersection, pdf);
}