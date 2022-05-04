//
// Created by bhiaibogf on 2022/5/1.
//

#ifndef PATH_TRACER_PARAMETER_H
#define PATH_TRACER_PARAMETER_H


#include "texture.h"

template<class T>
class Parameter;

template<class T>
std::ostream &operator<<(std::ostream &os, const Parameter<T> &parameter);

template<class T>
class Parameter {
public:
    explicit Parameter(const T &value);

    explicit Parameter(const std::string &path);

    ~Parameter() = default;

    void Prepare(const global::TexCoord &tex_coord);

    auto value() const { return value_; }

    friend std::ostream &operator<< <>(std::ostream &os, const Parameter &parameter);

private:
    T value_;
    Texture<T> *texture_ = nullptr;

    T GetValue(const global::TexCoord &tex_coord) const;

};


#endif //PATH_TRACER_PARAMETER_H
