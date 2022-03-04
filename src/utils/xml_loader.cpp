//
// Created by bhiaibogf on 2022/2/24.
//

#include "xml_loader.h"

XmlLoader::XmlLoader(const std::string &file_name) {
    doc_.LoadFile(file_name.c_str());
}

Camera *XmlLoader::LoadCamera() {
    auto camera = doc_.FirstChildElement("camera");

    global::Vector eye, lookat, up;
    eye = LoadVector(camera->FirstChildElement("eye"));
    lookat = LoadVector(camera->FirstChildElement("lookat"));
    up = LoadVector(camera->FirstChildElement("up"));

    float fovy = camera->FirstChildElement("fovy")->FirstAttribute()->FloatValue();
    int width = camera->FirstChildElement("width")->FirstAttribute()->IntValue();
    int height = camera->FirstChildElement("height")->FirstAttribute()->IntValue();

    return new Camera(eye, lookat, up, fovy, width, height);
}

void XmlLoader::LoadLights(std::vector<global::Vector> *lights) const {
    for (auto light = doc_.FirstChildElement("light"); light; light = light->NextSiblingElement("light")) {
        global::Vector radiance = LoadVector(light->Attribute("radiance"));
        lights->push_back(radiance);
    }
}

void XmlLoader::LoadSphere(std::vector<Sphere *> *spheres) const {
    for (auto sphere = doc_.FirstChildElement("sphere"); sphere; sphere = sphere->NextSiblingElement("sphere")) {
        global::Vector center = LoadVector(sphere->Attribute("center"));
        float radius = sphere->FindAttribute("radius")->FloatValue();
        std::string mtlname = sphere->Attribute("mtlname");
        auto *object = new Object(mtlname);
        spheres->push_back(new Sphere(object, center, radius));
    }
}

global::Vector XmlLoader::LoadVector(const tinyxml2::XMLElement *element) {
    return LoadVector(element->FirstAttribute()->Value());
}

global::Vector XmlLoader::LoadVector(const std::string &string) {
    std::stringstream ss(string);
    float x, y, z;
    char c;
    ss >> x >> c >> y >> c >> z;
    return {x, y, z};
}
