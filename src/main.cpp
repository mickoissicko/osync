#include "../lib/gethome/gethome.h"
#include "../include/stddef.h"

#include <filesystem>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fstream>

int main(int argc, char* argv[])
{
    ChangeDir(gethome(), 1);

    std::cout << "Checking config file...\n";

    if (!std::filesystem::exists(STD_FOLDER))
        if (!std::filesystem::create_directories(STD_FOLDER))
            return (ThrowError("Error making dir", MK_DIR));

    ChangeDir(STD_FOLDER, 1);

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

    snprintf(InitialDir, sizeof(InitialDir), "%s/%s", gethome(), STD_FOLDER);
    Parse(InitialDir);

    Config.close();

    char ServerDir[(MAX_PATH_LENGTH + strlen(STD_SERVER_FOLDER))];
    snprintf(ServerDir, sizeof(ServerDir), "%s/%s", InitialDir, STD_SERVER_FOLDER);

    char ParsedDir[(MAX_PATH_LENGTH + strlen(PREF_DIR))];
    snprintf(ParsedDir, sizeof(ParsedDir), "%s/%s", InitialDir, PREF_DIR);

    if (argc < 2) Client(ParsedDir);
    if (!strcmp(argv[1], "--server")) Server(ServerDir);

    return 0;
}
