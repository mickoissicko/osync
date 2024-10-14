#include "../include/imports.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
        return 1;

    if (!strcmp(argv[1], "--server")) Run(argv[1]);
    else Run(NULL);

    return 0;
}
