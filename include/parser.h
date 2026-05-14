#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <vector>
#include <iomanip>

namespace OP
{
    template<typename T>
    struct Vec3
    {
        T x;
        T y;
        T z;

        Vec3() : x(0), y(0), z(0) { }
        Vec3(T x, T y, T z) : x(x), y(y), z(z) { }

        friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& v) {
            os << "[ " << std::setw(6) << v.x << ", " << std::setw(6) << v.y << ", " << std::setw(6) << v.z << " ]";
            return os;
        }
    };

    template<typename T>
    struct Vec2
    {
        T x;
        T y;

        Vec2() : x(0), y(0) { }
        Vec2(T x, T y) : x(x), y(y) { }

        friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& v) {
            os << "[ " << std::setw(6) << v.x << ", " << std::setw(6) << v.y << " ]";
            return os;
        }
    };

    struct Vertex
    {
        Vec3<float> position;
        Vec2<float> uv;
        Vec3<float> normal;
    };

    std::vector<Vertex> parse(std::string path);
}

#endif
