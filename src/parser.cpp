#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>

#include "parser.h"

static std::string get_word(std::string line, int& pos)
{
    int length = line.size();
    int count = 0;
    int start_pos = pos;
    while (pos < length && line[pos++] != '\0') count++;
    return line.substr(start_pos, count);
}

static std::string get_word_until(std::string line, int& pos, char separator)
{
    int length = line.size();
    int count = 0;
    int start_pos = pos;
    while (pos < length && line[pos++] != separator) count++;
    return line.substr(start_pos, count);
}

namespace OP
{
    static std::vector<Vec3<float>> v_position;
    static std::vector<Vec2<float>> v_uv;
    static std::vector<Vec3<float>> v_normal;

    static SubMesh*  curr_sub_mesh = nullptr;
    static Mesh*     curr_mesh = nullptr;
    static Material* curr_mat = nullptr;

    static std::unordered_map<std::string, Material> materials;

    static Model model;

    static void parse_position(std::string line, int pos)
    {
        float x, y, z;
        std::string s_vertex;
        s_vertex = get_word_until(line, pos, ' ');
        x = std::stof(s_vertex);
        s_vertex = get_word_until(line, pos, ' ');
        y = std::stof(s_vertex);
        s_vertex = get_word_until(line, pos, ' ');
        z = std::stof(s_vertex);

        v_position.push_back(Vec3<float>(x, y, z));
    }

    static void parse_uv(std::string line, int pos)
    {
        float x, y;
        std::string s_uv;
        s_uv = get_word_until(line, pos, ' ');
        x = std::stof(s_uv);
        s_uv = get_word_until(line, pos, ' ');
        y = std::stof(s_uv);

        v_uv.push_back(Vec2<float>(x, y));
    }

    static void parse_normal(std::string line, int pos)
    {
        float x, y, z;
        std::string s_normal;
        s_normal = get_word_until(line, pos, ' ');
        x = std::stof(s_normal);
        s_normal = get_word_until(line, pos, ' ');
        y = std::stof(s_normal);
        s_normal = get_word_until(line, pos, ' ');
        z = std::stof(s_normal);

        v_normal.push_back(Vec3<float>(x, y, z));
    }

    static void parse_face(std::string line, int pos)
    {
        // ONLY SUPPORT TRIANGULATED MESHES, IF IT ISN'T, IT WILL ---DEFINITELY--- BREAK
        std::string s_face;
        Vertex vertex;

        int count;

        if (!curr_sub_mesh)
        {
            curr_mesh->sub_meshes.push_back(SubMesh());
            curr_sub_mesh = &curr_mesh->sub_meshes.back();
        }

        count = 0;
        s_face = get_word_until(line, pos, ' ');
        vertex.position = v_position[std::stoi(get_word_until(s_face, count, '/')) - 1];
        vertex.uv = v_uv[std::stoi(get_word_until(s_face, count, '/')) - 1];
        vertex.normal = v_normal[std::stoi(get_word_until(s_face, count, '/')) - 1];

        curr_sub_mesh->vertices.push_back(vertex);

        count = 0;
        s_face = get_word_until(line, pos, ' ');
        vertex.position = v_position[std::stoi(get_word_until(s_face, count, '/')) - 1];
        vertex.uv = v_uv[std::stoi(get_word_until(s_face, count, '/')) - 1];
        vertex.normal = v_normal[std::stoi(get_word_until(s_face, count, '/')) - 1];

        curr_sub_mesh->vertices.push_back(vertex);

        count = 0;
        s_face = get_word_until(line, pos, ' ');
        vertex.position = v_position[std::stoi(get_word_until(s_face, count, '/')) - 1];
        vertex.uv = v_uv[std::stoi(get_word_until(s_face, count, '/')) - 1];
        vertex.normal = v_normal[std::stoi(get_word_until(s_face, count, '/')) - 1];

        curr_sub_mesh->vertices.push_back(vertex);
    }

    void parse_mtl(std::string mtl_path)
    {
        std::ifstream mtl_file(mtl_path);

        if (!mtl_file.is_open())
        {
            printf("Unable to open file %s\n", mtl_path.c_str());
            exit(1);
        }

        std::string line;

        while (getline(mtl_file, line))
        {
            if (line[0] == '#') continue; // comment, skip

            int pos = 0;
            int line_len = line.length();

            std::string elem = get_word_until(line, pos, ' ');

            if (elem == "newmtl")
            {
                std::string mat_name = get_word(line, pos);
                materials[mat_name] = Material();
                curr_mat = &materials[mat_name];
            }
            else if (elem == "Ns")
                curr_mat->shininess = std::stof(get_word(line, pos));
            else if (elem == "Ka")
            {
                curr_mat->ambient.x = std::stof(get_word_until(line, pos, ' '));
                curr_mat->ambient.y = std::stof(get_word_until(line, pos, ' '));
                curr_mat->ambient.z = std::stof(get_word_until(line, pos, ' '));
            }
            else if (elem == "Kd")
            {
                curr_mat->diffuse.x = std::stof(get_word_until(line, pos, ' '));
                curr_mat->diffuse.y = std::stof(get_word_until(line, pos, ' '));
                curr_mat->diffuse.z = std::stof(get_word_until(line, pos, ' '));
            }
            else if (elem == "Ks")
            {
                curr_mat->specular.x = std::stof(get_word_until(line, pos, ' '));
                curr_mat->specular.y = std::stof(get_word_until(line, pos, ' '));
                curr_mat->specular.z = std::stof(get_word_until(line, pos, ' '));
            }
            else if (elem == "Ke")
            {
                curr_mat->emission.x = std::stof(get_word_until(line, pos, ' '));
                curr_mat->emission.y = std::stof(get_word_until(line, pos, ' '));
                curr_mat->emission.z = std::stof(get_word_until(line, pos, ' '));
            }
            else if (elem == "Ni")
                curr_mat->optical_density = std::stof(get_word(line, pos));
            else if (elem == "d")
                curr_mat->dissolve = std::stof(get_word(line, pos));
            else if (elem == "Tr")
                curr_mat->dissolve = 1 - std::stof(get_word(line, pos)); // Tr is inversed
            else if (elem == "illum")
                curr_mat->model = std::stoi(get_word(line, pos));
            else if (elem == " " || elem == "\0")
                ;
            else
            {
                printf("Element '%s' not implemented yet.\n", elem.c_str());
            }
        }
    }

    Model parse(std::string path)
    {
        std::filesystem::path obj_path = path;
        std::filesystem::path dir = obj_path.parent_path();

        std::ifstream obj_file(obj_path);

        if (!obj_file.is_open())
        {
            printf("Unable to open file %s\n", obj_path.string().c_str());
            exit(1);
        }

        std::string line;

        while (getline(obj_file, line))
        {
            if (line[0] == '#') continue; // comment, skip

            int pos = 0;
            int line_len = line.length();

            std::string elem = get_word_until(line, pos, ' ');

            if (elem == "v")
                parse_position(line, pos);
            else if (elem == "vt")
                parse_uv(line, pos);
            else if (elem == "vn")
                parse_normal(line, pos);
            else if (elem == "s")
            {
                std::string smooth = get_word(line, pos);
                
                if (smooth == "0" || smooth == "off")
                    curr_mesh->smooth = false;
                else
                    curr_mesh->smooth = true;
            }
            else if (elem == "f")
                parse_face(line, pos);
            else if (elem == "o")       // TODO: store mesh name
            {
                model.meshes.push_back(Mesh());
                curr_mesh = &model.meshes.back();
            }
            else if (elem == "usemtl")  // TODO: use specific material for mesh
            {
                // when i see usemtl, go to the next mesh and set material
                std::string material_name = get_word(line, pos);
                curr_mesh->sub_meshes.push_back(SubMesh());
                curr_sub_mesh = &curr_mesh->sub_meshes.back();
                curr_sub_mesh->material = &materials[material_name];
            }
            else if (elem == "mtllib")  // TODO: load material library
            {
                std::string mtl_name = get_word(line, pos);
                std::filesystem::path mtl_path = dir / mtl_name;
                parse_mtl(mtl_path.string());
            }
            else if (elem == "g")       // TODO: grouping
                ;
            else
            {
                printf("Element %s not implemented yet.\n", elem.c_str());
            }
        }

        obj_file.close();

        v_position.clear();
        v_uv.clear();
        v_normal.clear();

        return model;
    }
}
