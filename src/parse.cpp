#include "../include/stddef.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

void Parse(char ParseDir[])
{
    ChangeDir(ParseDir, 1);

    Tracker.close();
}
