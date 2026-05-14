#include <iostream>

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

    OP::parse(input_path);
}
