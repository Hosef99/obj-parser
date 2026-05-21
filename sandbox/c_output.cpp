#include <iostream>
#include <filesystem>

#include "parser.h"

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 3)
    {
        std::cerr << "Usage: obj-parser [obj file] [output file (optional)]" << std::endl;
        return 1;
    }

    std::string output_dir = "output";
    std::string output_path = "output/out.c";

    std::filesystem::create_directories(output_dir);

    if (argc == 3)
        output_path = argv[2];

    OP::obj_codegen(argv[1], output_path);

    std::cout << "The output file: '" << output_path << "' is created successfully!" << std::endl;
}
