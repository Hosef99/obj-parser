#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>

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

static std::string get_word_until_space(std::string line, int& pos)
{
    int length = line.size();
    int count = 0;
    int start_pos = pos;
    while (pos < length && line[pos++] != ' ') count++;
    while (pos < length && line[pos] == ' ')
        pos++;
    return line.substr(start_pos, count);
}

namespace OP
{
    static void parse_position(ObjEnv& env, std::string line, int pos)
    {
        float x, y, z;
        std::string s_position;
        s_position = get_word_until_space(line, pos);
        x = std::stof(s_position);
        s_position = get_word_until_space(line, pos);
        y = std::stof(s_position);
        s_position = get_word_until_space(line, pos);
        z = std::stof(s_position);

        env.v_position.push_back(Vec3<float>(x, y, z));
    }

    static void parse_uv(ObjEnv& env, std::string line, int pos)
    {
        float x, y;
        std::string s_uv;
        s_uv = get_word_until_space(line, pos);
        x = std::stof(s_uv);
        s_uv = get_word_until_space(line, pos);
        y = std::stof(s_uv);

        env.v_uv.push_back(Vec2<float>(x, y));
    }

    static void parse_normal(ObjEnv& env, std::string line, int pos)
    {
        float x, y, z;
        std::string s_normal;
        s_normal = get_word_until_space(line, pos);
        x = std::stof(s_normal);
        s_normal = get_word_until_space(line, pos);
        y = std::stof(s_normal);
        s_normal = get_word_until_space(line, pos);
        z = std::stof(s_normal);

        env.v_normal.push_back(Vec3<float>(x, y, z));
    }

    static void parse_vertex(ObjEnv& env, std::string line, int& pos)
    {
        Vertex v;
        int count = 0;
        std::string s_face = get_word_until_space(line, pos);

        int slash_count = std::count(s_face.begin(), s_face.end(), '/');
        if (slash_count == 0) // position
        {
            v.position = env.v_position[std::stoi(s_face) - 1];
            v.uv = Vec2<float>();
            v.normal = Vec3<float>();
        }
        else if (slash_count == 1) // position + uv
        {
            v.position = env.v_position[std::stoi(get_word_until(s_face, count, '/')) - 1];
            v.uv = env.v_uv[std::stoi(get_word(s_face, count)) - 1];
            v.normal = Vec3<float>();
        }
        else if (slash_count == 2) // position + uv + normal OR position + normal
        {
            v.position = env.v_position[std::stoi(get_word_until(s_face, count, '/')) - 1];
            std::string s_uv = get_word_until(s_face, count, '/');
            if (s_uv == "") 
                v.uv = Vec2<float>();
            else 
                v.uv = env.v_uv[std::stoi(s_uv) - 1];
            v.normal = env.v_normal[std::stoi(get_word(s_face, count)) - 1];
        }
        else
        {
            std::cout << "Slashes more than 2 is not supported\n";
            exit(1);
        }

        env.curr_sub_mesh->vertices.push_back(v);
    }

    static void parse_face(ObjEnv& env, std::string line, int pos)
    {
        // ONLY SUPPORT TRIANGULATED MESHES, IF IT ISN'T, IT WILL ---DEFINITELY--- BREAK
        if (!env.curr_sub_mesh)
        {
            env.curr_mesh->sub_meshes.push_back(SubMesh());
            env.curr_sub_mesh = &env.curr_mesh->sub_meshes.back();
        }

        parse_vertex(env, line, pos);
        parse_vertex(env, line, pos);
        parse_vertex(env, line, pos);
    }

    void parse_mtl(ObjEnv& env, std::string mtl_path)
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

            std::string elem = get_word_until_space(line, pos);

            if (elem == "newmtl")
            {
                std::string mat_name = get_word(line, pos);
                env.materials[mat_name] = Material();
                env.curr_mat = &env.materials[mat_name];
            }
            else if (elem == "Ns")
                env.curr_mat->shininess = std::stof(get_word(line, pos));
            else if (elem == "Ka")
            {
                env.curr_mat->ambient.x = std::stof(get_word_until_space(line, pos));
                env.curr_mat->ambient.y = std::stof(get_word_until_space(line, pos));
                env.curr_mat->ambient.z = std::stof(get_word_until_space(line, pos));
            }
            else if (elem == "Kd")
            {
                env.curr_mat->diffuse.x = std::stof(get_word_until_space(line, pos));
                env.curr_mat->diffuse.y = std::stof(get_word_until_space(line, pos));
                env.curr_mat->diffuse.z = std::stof(get_word_until_space(line, pos));
            }
            else if (elem == "Ks")
            {
                env.curr_mat->specular.x = std::stof(get_word_until_space(line, pos));
                env.curr_mat->specular.y = std::stof(get_word_until_space(line, pos));
                env.curr_mat->specular.z = std::stof(get_word_until_space(line, pos));
            }
            else if (elem == "Ke")
            {
                env.curr_mat->emission.x = std::stof(get_word_until_space(line, pos));
                env.curr_mat->emission.y = std::stof(get_word_until_space(line, pos));
                env.curr_mat->emission.z = std::stof(get_word_until_space(line, pos));
            }
            else if (elem == "Ni")
                env.curr_mat->optical_density = std::stof(get_word(line, pos));
            else if (elem == "d")
                env.curr_mat->dissolve = std::stof(get_word(line, pos));
            else if (elem == "Tr")
                env.curr_mat->dissolve = 1 - std::stof(get_word(line, pos)); // Tr is inversed
            else if (elem == "illum")
                env.curr_mat->model = std::stoi(get_word(line, pos));
            else if (elem == " " || elem == "\0" || elem == "")
                ;
            else
            {
                printf("Element '%s' not implemented yet.\n", elem.c_str());
            }
        }
    }

    Model parse(std::string path)
    {
        ObjEnv env;

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

            std::string elem = get_word_until_space(line, pos);

            if (elem == "v")
                parse_position(env, line, pos);
            else if (elem == "vt")
                parse_uv(env, line, pos);
            else if (elem == "vn")
                parse_normal(env, line, pos);
            else if (elem == "s")
            {
                std::string smooth = get_word(line, pos);
                
                if (smooth == "0" || smooth == "off")
                    env.curr_mesh->smooth = false;
                else
                    env.curr_mesh->smooth = true;
            }
            else if (elem == "f")
                parse_face(env, line, pos);
            else if (elem == "o")       // TODO: store mesh name
            {
                env.model.meshes.push_back(Mesh());
                env.curr_mesh = &env.model.meshes.back();
            }
            else if (elem == "usemtl")  // TODO: use specific material for mesh
            {
                // when i see usemtl, go to the next mesh and set material
                std::string material_name = get_word(line, pos);
                env.curr_mesh->sub_meshes.push_back(SubMesh());
                env.curr_sub_mesh = &env.curr_mesh->sub_meshes.back();
                env.curr_sub_mesh->material = env.materials[material_name];
            }
            else if (elem == "mtllib")  // TODO: load material library
            {
                std::string mtl_name = get_word(line, pos);
                std::filesystem::path mtl_path = dir / mtl_name;
                parse_mtl(env, mtl_path.string());
            }
            else if (elem == "g")       // TODO: grouping
                ;
            else
            {
                printf("Element %s not implemented yet.\n", elem.c_str());
            }
        }

        obj_file.close();

        return env.model;
    }
}
