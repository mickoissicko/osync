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

int Tokenise(char* StartingDirectory)
{
    ChangeDir(StartingDir, 1);

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

        ChangeDir(PREF_DIR, 1);

        for (const auto& [Key, Value] : ServerConfig)
        {
            char KeyFileName[MAX_FILENAME_LENGTH];
            snprintf(KeyFileName, sizeof(KeyFileName), "%s%s", Key, DEF_FMT);

            FILE* TrackFile = fopen(TRACK_FILE, "a+");
            std::ofstream KeyFile(KeyFileName);

            if (!KeyFile.is_open() || !TrackFile) std::exit((ThrowError("Error opening file", OPN_FILE)));

            fprintf(TrackFile, "%s%s\n", Key, DEF_FMT);
            KeyFile << Value << '\n';

            // key (arg) -> value (input)
            // std::cout << Key << ": " << Value << '\n';

            fclose(TrackFile);
            KeyFile.close();

            Index++;
        }
    }

    else std::exit((ThrowError("Invalid config"), ERR_PARSE));
    return DEF_OK;
}
