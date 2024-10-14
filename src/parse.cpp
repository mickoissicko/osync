#include "../include/stdtokens.h"
#include "../include/stddef.h"

#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <string>

std::string Trim(const std::string& String)
{
    size_t First = String.find_first_not_of(' ');
    size_t Last = String.find_last_not_of(' ');
    return (First == std::string::npos || Last == std::string::npos) ? "" : String.substr(First, Last - First + 1);
}

bool IsWhitespace(const std::string& String)
{
    return String.find_first_not_of(' ') == std::string::npos;
}

void ProcessLine(const std::string& Line, std::unordered_map<std::string, std::string>& ServerConfig, std::string& CurrentKey)
{
    std::string CurrentLine = Trim(Line);

    if (CurrentLine.empty() || CurrentLine.find(COMMENT) == 0 || IsWhitespace(CurrentLine))
        return;

    if (CurrentLine.find(SERVER_TOKEN) != std::string::npos)
    {
        CurrentKey.clear();
        ServerConfig.clear();
        return;
    }

    size_t DelimPosition = CurrentLine.find(DELIMITER);

    if (DelimPosition != std::string::npos)
    {
        CurrentKey = Trim(CurrentLine.substr(0, DelimPosition));
        std::string Value = Trim(CurrentLine.substr(DelimPosition + DELIMITER.length()));

        if (!Value.empty())
        {
            ServerConfig[CurrentKey] = Value;
            CurrentKey.clear();
        }

    }

    else if (!CurrentKey.empty())
    {
        std::string Value = Trim(CurrentLine);

        if (!Value.empty())
        {
            ServerConfig[CurrentKey] = Value;
            CurrentKey.clear();
        }
    }
}

int Parse(char* StartingDirectory)
{
    if (chdir(StartingDirectory) != 0) std::exit((ThrowError("Could not change directory", OPN_DIR)));

    int Index = 1;

    std::unordered_map<std::string, std::string> ServerConfig;
    std::ifstream Config(CONFIG);
    std::string CurrentKey;

    if (!Config) std::exit((ThrowError("Error whilst opening file"), OPN_FILE));

    std::string Line;

    while (std::getline(Config, Line))
        ProcessLine(Line, ServerConfig, CurrentKey);

    Config.close();

    if (!ServerConfig.empty())
    {
        if (!std::filesystem::exists(PREF_DIR))
            if (!std::filesystem::create_directories(PREF_DIR)) std::exit((ThrowError("Error making dir", MK_DIR)));

        if (chdir(PREF_DIR) != 0) std::exit((ThrowError("Error whilst changing directory", OPN_DIR)));

        for (const auto& [Key, Value] : ServerConfig)
        {
            char KeyFileName[MAX_FILENAME_LENGTH];

            snprintf(KeyFileName, sizeof(KeyFileName), "%s%s", Key, TXT);

            std::ofstream TrackFile(TRACK_FILE);
            std::ofstream KeyFile(KeyFileName);

            if (!KeyFile.is_open() || !TrackFile.is_open()) std::exit((ThrowError("Error opening file", OPN_FILE)));

            TrackFile << Index << '\n';
            KeyFile << Value << '\n';

            // key (arg) -> value (input)
            // std::cout << Key << ": " << Value << '\n';

            TrackFile.close();
            KeyFile.close();
            Index++;
        }
    }

    else std::exit((ThrowError("Invalid config"), ERR_PARSE));
    return DEF_OK;
}