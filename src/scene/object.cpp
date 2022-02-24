//
// Created by bhiaibogf on 2022/2/21.
//

#include "object.h"

Object::Object(Mesh *mesh, Material *material) : mesh_(mesh), material_(material) {
    std::cout << "  " << *mesh << std::endl;
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
}

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
