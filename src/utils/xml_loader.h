//
// Created by bhiaibogf on 2022/2/24.
//

#ifndef PATH_TRACER_XML_LOADER_H
#define PATH_TRACER_XML_LOADER_H


#include "../camera/camera.h"
#include "../scene/object.h"

#include "tinyxml2/tinyxml2.h"

class XmlLoader {
public:
    explicit XmlLoader(const std::string &file_name);

    ~XmlLoader() = default;

    Camera *LoadCamera();

    void LoadLights(std::map<std::string, global::Vector> *lights) const;

    void LoadSphere(std::vector<Sphere *> *spheres) const;

private:
    tinyxml2::XMLDocument doc_;

    static global::Vector LoadVector(const tinyxml2::XMLElement *element);

    static global::Vector LoadVector(const std::string &string);

};


#endif //PATH_TRACER_XML_LOADER_H
