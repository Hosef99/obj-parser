#include <iostream>
#include <fstream>
#include <string>

#include "parser.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: obj-parser [input-file] [output-file]\n");
        return 1;
    }

    char *input_path  = argv[1];
    char *output_path = argv[2];

    std::ifstream input_file(input_path);


    if (!input_file.is_open())
    {
        printf("Unable to open file %s\n", input_path);
        return 1;
    }

    std::string line;

    while (getline(input_file, line))
    {
        if (line[0] == '#') continue; // comment

        int pos = 0;
        int line_len = line.length();

        while (line[pos] != ' ')
            pos++;

        std::string elem = line.substr(0, pos);

        std::cout << elem << std::endl;
    }

    input_file.close();
}
