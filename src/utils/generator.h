//
// Created by bhiaibogf on 2022/2/24.
//

#ifndef PATH_TRACER_GENERATOR_H
#define PATH_TRACER_GENERATOR_H


#include <random>

namespace generator {
    namespace {
        std::random_device rd_;
        std::default_random_engine random_engine_(rd_());
        std::uniform_real_distribution<float> uniform_distribution_(0.f, 1.f);
        std::normal_distribution<float> normal_distribution_(0.f, 0.5f);
    }

    float Rand();

    float RandN();

}


#endif //PATH_TRACER_GENERATOR_H
