//
// Created by bhiaibogf on 2022/2/18.
//

#include "funcs.h"

float funcs::Radius(float alpha) {
    return alpha / 180.f * kPi;
}

void funcs::UpdateProgress(float progress) {
    std::cout << "\r[";

    int bar_width = 70;
    int pos = bar_width * progress;
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }

    std::cout << "] " << int(progress * 100.0) << " %";
    std::cout.flush();
}

float funcs::RandomFloat() {
    return uniform_(random_engine_);
}
