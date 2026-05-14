#include <iostream>
#include <string>
#include <fstream>
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

    static SubMesh* curr_sub_mesh = nullptr;
    static Mesh*    curr_mesh = nullptr;

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

    Model parse(std::string path)
    {
        std::string obj_path = path + ".obj";
        std::string mtl_path = path + ".mtl";

        // TODO: parse mtl file before obj file

        std::ifstream obj_file(obj_path);

        if (!obj_file.is_open())
        {
            printf("Unable to open file %s\n", obj_path);
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
                // curr_sub_mesh->material = &materials[material_name];
            }
            else if (elem == "mtllib")  // TODO: load material library
                ;
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
