//
// Created by bhiaibogf on 2022/2/21.
//

#include "triangle.h"

Triangle::Triangle(Object *object,
                   global::VectorArray *vertices, global::VectorArray *normals,
                   global::TexCoordArray *tex_coords)
        : Primitive(object), vertices_(*vertices) {
    global::Vector normal;
    normal = (vertices_[1] - vertices_[0]).cross(vertices_[2] - vertices_[0]);
    area_ = normal.norm() / 2.f;

    if (normals) {
        normals_ = *normals;
    } else {
        normal.normalize();
        for (auto &normal_: normals_) {
            normal_ = normal;
        }
    }
    if (tex_coords) {
        tex_coords_ = *tex_coords;
    } else {
        for (auto &tex_coord: tex_coords_) {
            tex_coord = global::kNoTex;
        }
    }

    for (auto &vertex: vertices_) {
        bound_ += vertex;
    }
}

// // Moller Trumbore
// // intersect to only one face
// bool Triangle::Intersect(Ray *ray, Intersection *intersection) const {
//     auto &v0 = vertices_[0], &v1 = vertices_[1], &v2 = vertices_[2];
//     global::Vector e1 = v1 - v0, e2 = v2 - v0;
//
//     global::Vector p_vec = ray->direction().cross(e2);
//     float det = e1.dot(p_vec);
//     if (det <= 0) {
//         return false;
//     }
//
//     global::Vector t_vec = ray->origin() - v0;
//     float u = t_vec.dot(p_vec);
//     if (u < 0 || u > det) {
//         return false;
//     }
//
//     global::Vector q_vec = t_vec.cross(e1);
//     float v = q_vec.dot(ray->direction());
//     if (v < 0 || u + v > det) {
//         return false;
//     }
//
//     float inv_det = 1 / det;
//
//     float t = e2.dot(q_vec) * inv_det;
//     if (ray->Update(t)) {
//         u *= inv_det;
//         v *= inv_det;
//         intersection->position = (1 - u - v) * vertices_[0] + u * vertices_[1] + v * vertices_[2];;
//         intersection->normal = (1 - u - v) * normals_[0] + u * normals_[1] + v * normals_[2];
//         intersection->tex_coord = (1 - u - v) * tex_coords_[0] + u * tex_coords_[1] + v * tex_coords_[2];
//         intersection->material = material();
//         intersection->weight = Weight();
//         return true;
//     }
//     return false;
// }

// 6.837 intersect to both faces
bool Triangle::Intersect(Ray *ray, Intersection *intersection) const {
    float aa[] = {vertices_[0].x() - vertices_[1].x(), vertices_[0].x() - vertices_[2].x(), ray->direction().x(), 0.f,
                  vertices_[0].y() - vertices_[1].y(), vertices_[0].y() - vertices_[2].y(), ray->direction().y(), 0.f,
                  vertices_[0].z() - vertices_[1].z(), vertices_[0].z() - vertices_[2].z(), ray->direction().z(), 0.f,
                  0.f, 0.f, 0.f, 1.f};
    Eigen::Matrix4f A(aa);
    A.transposeInPlace();

    Eigen::Vector4f b{vertices_[0].x() - ray->origin().x(), vertices_[0].y() - ray->origin().y(),
                      vertices_[0].z() - ray->origin().z(), 0.f};

    if (A.determinant() == 0) {
        return false;
    }

    b = A.inverse() * b;

    float beta = b[0], gamma = b[1], t = b[2];
    if (beta + gamma <= 1 && beta >= 0 && gamma >= 0 && ray->Update(t)) {
        intersection->position = (1 - beta - gamma) * vertices_[0] + beta * vertices_[1] + gamma * vertices_[2];;
        intersection->normal = (1 - beta - gamma) * normals_[0] + beta * normals_[1] + gamma * normals_[2];
        intersection->tex_coord = (1 - beta - gamma) * tex_coords_[0] + beta * tex_coords_[1] + gamma * tex_coords_[2];
        intersection->material = material();
        intersection->weight = Weight();
        return true;
    }
    return false;
}

void Triangle::Sample(Intersection *intersection, float *pdf) const {
    float xi_1 = std::sqrt(generator::Rand()), xi_2 = generator::Rand();
    intersection->position =
            vertices_[0] * (1.f - xi_1) + vertices_[1] * (xi_1 * (1.f - xi_2)) + vertices_[2] * (xi_1 * xi_2);
    intersection->normal =
            normals_[0] * (1.f - xi_1) + normals_[1] * (xi_1 * (1.f - xi_2)) + normals_[2] * (xi_1 * xi_2);
    intersection->normal.normalize();
    intersection->weight = Weight();
    *pdf = Weight();
}

void Triangle::InsertTo(std::vector<const Primitive *> *primitives) const {
    primitives->push_back(this);
}
