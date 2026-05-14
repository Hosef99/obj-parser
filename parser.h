#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <vector>

namespace OP
{
    template<typename T>
    struct Vec3
    {
        T x;
        T y;
        T z;
    };

    template<typename T>
    struct Vec2
    {
        T x;
        T y;
    };

    struct Vertex
    {
        Vec3<float> pos;
        Vec2<float> uv;
        Vec3<float> normal;
    };

    std::vector<Vertex> parse(std::string path);
}

#endif
