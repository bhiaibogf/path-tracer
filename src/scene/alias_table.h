//
// Created by bhiaibogf on 2022/3/3.
//

#ifndef PATH_TRACER_ALIAS_TABLE_H
#define PATH_TRACER_ALIAS_TABLE_H


#include "object.h"
#include "../utils/generator.h"

class AliasTable {
public:
    explicit AliasTable(const std::vector<Object *> &objects);

    void SampleLight(Intersection *intersection, float *pdf) const;

private:
    std::vector<const Primitive *> light_primitives_;
    std::vector<int> indices_;

    std::vector<float> probabilities_;
    std::vector<int> alias_;

    float area_weighted_;
    int n_;

    void CreateTable();

};


#endif //PATH_TRACER_ALIAS_TABLE_H
