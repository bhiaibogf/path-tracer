//
// Created by bhiaibogf on 2022/2/22.
//

#include "obj_loader.h"

ObjLoader::ObjLoader(const std::string &model_path, const std::string &model_name) {
    model_path_ = model_path + model_name + "/";
    filename_ = model_path_ + model_name + ".obj";

    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = model_path_;

    if (!reader_.ParseFromFile(filename_, reader_config)) {
        if (!reader_.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader_.Error();
        }
        exit(1);
    }
    if (!reader_.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader_.Warning();
    }
}

void ObjLoader::Load(const std::map<std::string, global::Vector> &lights,
                     const std::vector<Sphere *> &spheres,
                     Scene *scene) {
    LoadMaterials(lights, spheres);
    LoadSpheres(scene, spheres);
    LoadMeshes(scene);
}

void ObjLoader::LoadMaterials(const std::map<std::string, global::Vector> &lights,
                              const std::vector<Sphere *> &spheres) {
    auto &materials = reader_.GetMaterials();
    for (const auto &material: materials) {
        std::string texture_path = model_path_;
        // diffuse
        global::Color k_d(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
        std::string diffuse_texture = material.diffuse_texname;
        // specular
        global::Color k_s(material.specular[0], material.specular[1], material.specular[2]);
        std::string specular_texture = material.specular_texname;
        float n_s = material.shininess;
        float n_i = material.ior;

        Material *material_;
        if (n_i != 1.f) {
            material_ = new Refraction(n_i);
        } else {
            if (k_s == global::kBlack && specular_texture.empty()) {
                if (!diffuse_texture.empty()) {
                    material_ = new Lambert(texture_path + diffuse_texture);
                } else {
                    material_ = new Lambert(k_d);
                }
            } else if (k_d == global::kBlack && diffuse_texture.empty()) {
                material_ = new Phong(k_s, n_s);
            } else {
                material_ = new Mix(
                        diffuse_texture.empty() ? new Lambert(k_d) : new Lambert(texture_path + diffuse_texture),
                        new Phong(k_s, n_s));
            }
            if (lights.contains(material.name)) {
                material_->SetEmission(lights.at(material.name));
            }
        }
        for (auto sphere: spheres) {
            if (material.name == sphere->object()->name()) {
                std::cout << "Loading " << material.name << "..." << std::endl;
                sphere->object()->SetPrimitive(sphere);
                sphere->object()->SetMaterial(material_);
            }
        }
        materials_.push_back(material_);
    }
}

void ObjLoader::LoadSpheres(Scene *scene, const std::vector<Sphere *> &spheres) {
    for (auto sphere: spheres) {
        scene->AddObject(sphere->object());
    }
}

void ObjLoader::LoadMeshes(Scene *scene) {
    auto &attrib = reader_.GetAttrib();
    auto &shapes = reader_.GetShapes();

    std::cout << "Loading ./" << filename_ << " (" << shapes.size() << " objects)" << std::endl;

    // Loop over shapes
    for (auto &shape: shapes) {
        auto *object = new Object(shape.name);
        std::cout << "Loading " << shape.name << "..." << std::endl;

        auto *mesh = new Mesh(object);

        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (auto vertices_count: shape.mesh.num_face_vertices) {
            std::array<global::Vector, 3> *vertices = new std::array<global::Vector, 3>;
            std::array<global::Vector, 3> *normals = nullptr;
            std::array<global::TexCoord, 3> *tex_coords = nullptr;

            assert(vertices_count == 3);
            // Loop over vertices in the face
            for (size_t idx_in_face = 0; idx_in_face < vertices_count; idx_in_face++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + idx_in_face];
                (*vertices)[idx_in_face] = global::Vector(attrib.vertices[3 * idx.vertex_index + 0],
                                                          attrib.vertices[3 * idx.vertex_index + 1],
                                                          attrib.vertices[3 * idx.vertex_index + 2]);

                if (idx.normal_index >= 0) {
                    if (!normals) {
                        normals = new std::array<global::Vector, 3>;
                    }
                    (*normals)[idx_in_face] = global::Vector(attrib.normals[3 * idx.normal_index + 0],
                                                             attrib.normals[3 * idx.normal_index + 1],
                                                             attrib.normals[3 * idx.normal_index + 2]);
                }

                if (idx.texcoord_index >= 0) {
                    if (!tex_coords) {
                        tex_coords = new std::array<global::TexCoord, 3>;
                    }
                    (*tex_coords)[idx_in_face] = global::TexCoord(attrib.texcoords[2 * idx.texcoord_index + 0],
                                                                  1.f - attrib.texcoords[2 * idx.texcoord_index + 1]);
                }
            }

            auto *triangle = new Triangle(object, vertices, normals, tex_coords);
            mesh->Add(triangle);
            index_offset += vertices_count;
        }
        object->SetPrimitive(mesh);
        object->SetMaterial(materials_[shape.mesh.material_ids[0]]);
        scene->AddObject(object);
    }
}
