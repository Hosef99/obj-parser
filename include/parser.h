#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <vector>
#include <iomanip>
#include <unordered_map>

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

    struct Material
    {
        Vec3<float> ambient;
        Vec3<float> diffuse;
        Vec3<float> specular;
        Vec3<float> emission;
        float       optical_density;    // Optical density
        float       shininess;  // Specular exponent
        float       dissolve;
        int         model;      // Illumination model
    };

    struct Vertex
    {
        Vec3<float> position;
        Vec2<float> uv;
        Vec3<float> normal;
    };

    struct SubMesh
    {
        std::vector<Vertex> vertices;
        Material material;
        std::string material_name;
    };

    struct Mesh
    {
        std::vector<SubMesh> sub_meshes;
        bool smooth;
        std::string name;
    };

    struct Model
    {
        std::vector<Mesh> meshes;
    };

    struct ObjEnv
    {
        std::vector<Vec3<float>> v_position;
        std::vector<Vec2<float>> v_uv;
        std::vector<Vec3<float>> v_normal;

        SubMesh*  curr_sub_mesh = nullptr;
        Mesh*     curr_mesh     = nullptr;
        Material* curr_mat      = nullptr;

        std::unordered_map<std::string, Material> materials;

        Model model;
    };

    Model parse(std::string path);

    void obj_codegen(std::string obj_path, std::string output_path);
}

#endif
