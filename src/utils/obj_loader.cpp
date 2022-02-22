//
// Created by bhiaibogf on 2022/2/22.
//

#include "obj_loader.h"

std::vector<Object> obj_loader::Load(const std::string &model_path, const std::string &model_name) {
    std::string filename = model_path + "/" + model_name + "/" + model_name + ".obj";

    tinyobj::ObjReaderConfig reader_config;
    // Path to material files
    reader_config.mtl_search_path = model_path + "/" + model_name + "/";
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }
    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    std::vector<Object> objects;
    // Loop over shapes
    for (auto &shape: shapes) {
        Mesh *mesh = new Mesh();
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (auto face: shape.mesh.num_face_vertices) {
            std::array<global::Vector, 3> vertices;
            std::array<global::Vector, 3> normals;
            std::array<global::TexCoord, 3> tex_coords;

            assert(face == 3);
            // Loop over vertices in the face
            for (size_t v = 0; v < face; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                vertices[v] = global::Vector(attrib.vertices[3 * idx.vertex_index + 0],
                                             attrib.vertices[3 * idx.vertex_index + 1],
                                             attrib.vertices[3 * idx.vertex_index + 2]);

                if (idx.normal_index >= 0) {
                    normals[v] = global::Vector(attrib.normals[3 * idx.normal_index + 0],
                                                attrib.normals[3 * idx.normal_index + 1],
                                                attrib.normals[3 * idx.normal_index + 2]);
                }

                if (idx.texcoord_index >= 0) {
                    tex_coords[v] = global::TexCoord(attrib.texcoords[2 * idx.texcoord_index + 0],
                                                     attrib.texcoords[2 * idx.texcoord_index + 1]);
                }
            }

            auto *triangle = new Triangle(vertices, normals, tex_coords);
            mesh->Add(triangle);
            index_offset += face;
        }

        shape.mesh.material_ids[0];
        Material *material = new Phong({}, {}, {}, {});

        objects.emplace_back(mesh, material);
    }
    return objects;
}
