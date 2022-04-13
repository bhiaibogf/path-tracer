//
// Created by bhiaibogf on 2022/3/3.
//

#include "alias_table.h"

#include <queue>

AliasTable::AliasTable(const std::vector<Object *> &objects) {
    for (auto &object: objects) {
        if (object->material()->HasEmitter()) {
            object->InsertTo(&light_primitives_);
        }
    }
    n_ = light_primitives_.size();
    indices_.resize(n_);

    area_weighted_ = 0.f;
    for (auto light_primitive: light_primitives_) {
        area_weighted_ += light_primitive->AreaWeighted();
    }

    probabilities_.resize(n_);
    alias_.resize(n_);

    CreateTable();
}

void AliasTable::CreateTable() {
    std::queue<std::pair<int, float>> greater, less;
    for (int i = 0; i < n_; i++) {
        float pdf = light_primitives_[i]->AreaWeighted() / area_weighted_ * float(n_);
        if (pdf > 1.f) {
            greater.push({i, pdf});
        } else {
            less.push({i, pdf});
        }
    }

    int cnt = 0;
    while (!less.empty() && !greater.empty()) {
        probabilities_[cnt] = less.front().second;
        indices_[cnt] = less.front().first;
        less.pop();

        auto res = greater.front();
        greater.pop();

        alias_[cnt] = res.first;
        res.second -= (1.f - probabilities_[cnt]);
        if (res.second > 1.f) {
            greater.push(res);
        } else {
            less.push(res);
        }

        cnt++;
    }

    while (!greater.empty()) {
        probabilities_[cnt] = 1.f;
        indices_[cnt] = greater.front().first;
        greater.pop();
        cnt++;
    }
    while (!less.empty()) {
        probabilities_[cnt] = 1.f;
        indices_[cnt] = less.front().first;
        less.pop();
        cnt++;
    }
}

void AliasTable::SampleLight(Intersection *intersection, float *pdf) const {
    if (n_ == 0) {
        *pdf = 0.f;
        return;
    }
    float xi = generator::Rand();
    int i = int(float(n_) * xi);
    float y = float(n_) * xi - float(i);
    if (y < probabilities_[i]) {
        light_primitives_[indices_[i]]->Sample(intersection, pdf);
    } else {
        light_primitives_[alias_[i]]->Sample(intersection, pdf);
    }
    (*pdf) /= area_weighted_;
}

