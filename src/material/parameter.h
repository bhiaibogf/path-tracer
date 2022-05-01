//
// Created by bhiaibogf on 2022/5/1.
//

#ifndef PATH_TRACER_PARAMETER_H
#define PATH_TRACER_PARAMETER_H


#include "texture.h"

template<class T>
class Parameter {
public:
    explicit Parameter(const T &value);

    explicit Parameter(const std::string &path);

    ~Parameter() = default;

    T GetValue() const;

    T GetValue(global::TexCoord uv) const;

    // TODO
    friend std::ostream &operator<<(std::ostream &os, const Parameter<T> &parameter) {
        if (parameter.texture_) {
            os << parameter.texture_->path();
        } else {
            using namespace global;
            os << parameter.value_;
        }
        return os;
    }

private:
    T value_;
    Texture<T> *texture_ = nullptr;

};


#endif //PATH_TRACER_PARAMETER_H
