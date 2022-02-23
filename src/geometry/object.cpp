//
// Created by bhiaibogf on 2022/2/21.
//

#include "object.h"

Object::Object(Mesh *mesh, Material *material) : mesh_(mesh), material_(material) {
    std::cout << "    " << *mesh << std::endl;
    std::cout << "    " << *(Lambert *) material << std::endl;
}
