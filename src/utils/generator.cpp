//
// Created by bhiaibogf on 2022/2/24.
//

#include "generator.h"

float generator::Rand() {
    return uniform_distribution_(random_engine_);
}

float generator::RandN() {
    return normal_distribution_(random_engine_);
}
