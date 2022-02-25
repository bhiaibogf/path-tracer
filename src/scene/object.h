//
// Created by bhiaibogf on 2022/2/21.
//

#ifndef PATH_TRACER_OBJECT_H
#define PATH_TRACER_OBJECT_H


#include "../geometry/mesh.h"
#include "../material/lambert.h"
#include "../material/phong.h"
#include "../material/refraction.h"

class Object {
public:
    Object() = default;

    virtual ~Object() = default;

    void SetMesh(Mesh *mesh) {
        std::cout << "  " << *mesh << std::endl;
        mesh_ = mesh;
    }

    void SetMaterial(Material *material) {
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

    auto material() const { return material_; }

    auto Area() const { return mesh_->area(); }

    bool Intersect(Ray *ray, Intersection *intersection) const;

    void Sample(Intersection *intersection, float *pdf) const;

    void InsertTo(std::vector<Primitive *> *primitives) const { mesh_->InsertTo(primitives); }

private:
    Mesh *mesh_;
    Material *material_;

};


#endif //PATH_TRACER_OBJECT_H
