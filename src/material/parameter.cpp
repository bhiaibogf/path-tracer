//
// Created by bhiaibogf on 2022/5/1.
//

#include "parameter.h"

template
class Parameter<float>;

template
class Parameter<global::Color>;

template<class T>
Parameter<T>::Parameter(const T &value) : value_(value) {}

template<class T>
Parameter<T>::Parameter(const std::string &path) {
    texture_ = new Texture<T>(path);

    T sum;
    if constexpr (std::is_same<T, float>::value) {
        sum = 0.f;
    } else {
        sum = global::kBlack;
    }

    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; j++) {
            sum += texture_->GetColor({float(i) / 100.f, float(j) / 100.f});
        }
    }

    value_ = sum / 1e4;
}

template<class T>
T Parameter<T>::GetValue() const {
    return value_;
}

template<class T>
T Parameter<T>::GetValue(global::TexCoord uv) const {
    if (texture_) {
        return texture_->GetColor(uv);
    } else {
        return GetValue();
    }
}
