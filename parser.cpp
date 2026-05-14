#include "parser.h"

namespace OP
{
    std::vector<Vertex> parse(std::string path)
    {
        std::string obj_file = path + ".obj";
        std::string mtl_file = path + ".mtl";

        // TODO: parse mtl file before obj file

        std::ifstream obj_file(path);

        if (!obj_file.is_open())
        {
            printf("Unable to open file %s\n", obj_path);
            return 1;
        }

        std::string line;

        while (getline(obj_file, line))
        {
            if (line[0] == '#') continue; // comment, skip

            int pos = 0;
            int line_len = line.length();

            while (line[pos] != ' ')
                pos++;

            std::string elem = line.substr(0, pos);

            switch (elem)
            {
                case "mtllib":  // TODO: load material library
                    break;
                case "o":       // TODO: store mesh data
                    break;
                case "v":       // TODO: store position data
                    break;
                case "vt":      // TODO: store texture data
                    break;
                case "vn":      // TODO: store normal data
                    break;
                case "s":       // TODO: set smooth shading
                    break;
                case "usemtl":  // TODO: use specific material for this mesh
                    break;
                case "f":       // TODO: create vector of Vertex 
                    break;
            }
        }

        input_file.close();
    }
}
