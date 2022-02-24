//
// Created by bhiaibogf on 2022/2/24.
//

#include "xml_loader.h"

XmlLoader::XmlLoader(const std::string &file_name) {
    doc_.LoadFile(file_name.c_str());
}

Camera *XmlLoader::LoadCamera() {
    auto ls = doc_.FirstChildElement("camera");

    global::Vector eye, lookat, up;
    eye = LoadVector(ls->FirstChildElement("eye"));
    lookat = LoadVector(ls->FirstChildElement("lookat"));
    up = LoadVector(ls->FirstChildElement("up"));

    float fovy = ls->FirstChildElement("fovy")->FirstAttribute()->FloatValue();
    int width = ls->FirstChildElement("width")->FirstAttribute()->IntValue();
    int height = ls->FirstChildElement("height")->FirstAttribute()->IntValue();

    return new Camera(eye, lookat, up, fovy, width, height);
}

global::Vector XmlLoader::LoadVector(const tinyxml2::XMLElement *element) {
    std::stringstream ss(element->FirstAttribute()->Value());
    float x, y, z;
    char c;
    ss >> x >> c >> y >> c >> z;
    return {x, y, z};
}
