//
// Created by bhiaibogf on 2022/1/23.
//

#include "scene.h"

Scene::Scene() {

}

Eigen::Vector3f Scene::Trace(const Ray &ray, int depth) const {
    return ray(1.f);
}
