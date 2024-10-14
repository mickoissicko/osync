#include "../lib/gethome/gethome.h"
#include "../include/stddef.h"

#include <filesystem>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fstream>

int main(int argc, char* argv[])
{
    if (chdir(gethome()) != 0)
        return (ThrowError("Error opening dir", OPN_DIR));

    std::cout << "Checking config file...\n";

    if (!std::filesystem::exists(STD_FOLDER))
        if (!std::filesystem::create_directories(STD_FOLDER))
            return (ThrowError("Error making dir", MK_DIR));

    if (chdir(STD_FOLDER) != 0)
        return (ThrowError("Error whilst opening dir"), OPN_DIR);

    std::cout << "Reading config...\n";

    std::ifstream Config(CONFIG);

    if (!Config.is_open())
    {
        std::ofstream _Config(CONFIG);

        if (!(_Config.is_open()))
            return (ThrowError("Error making config file"), MK_FILE);

        _Config.close();
        return 0;
    }

    std::cout << "Parsing config...\n";

    char InitialDir[MAX_PATH_LENGTH];

    snprintf(InitialDir, sizeof(InitialDir), "%s/%s/", gethome(), STD_FOLDER);
    Parse(InitialDir);

    Config.close();

    return 0;
}
