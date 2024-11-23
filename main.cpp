#include "incl/stddef.hpp"
#include "lib/gethome.h"
#include <filesystem>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <limits>

#define MAX_LEN 8192

namespace Fs = std::filesystem;

#define CLR ignore(std::numeric_limits<std::streamsize>::max(), '\n')

int Throw(int Err, int Hard)
{
    switch (Err)
    {
        case 1:
            std::cout << "Error whilst changing directory\n";
            std::cout << "Invalid path?\n";

            if (Hard)
                std::exit(Err);
            break;

        case 2:
            std::cout << "Git not installed\n";
            std::cout << "Please install Git\n";
            std::exit(Err);
            break;

        case 3:
            std::cout << "Error opening or writing (to) file\n";
            std::exit(Err);
            break;

        case 4:
            std::cout << "Could not clone repository (local)\n";
            std::exit(Err);
            break;

        default:
            break;
    }
}

void Process(const char* String, int Op)
{
    switch (Op)
    {
        case 1:
            if (chdir(String) != 0)
                Throw(1, 1);
            break;

        case 2:
            if (std::system(String) != 0)
                Throw(2, 1);
            break;

        default:
            break;
    }
}

void MakeVault()
{
    char Ui;

    std::cout << "Creating a vault location for you...\n";

    Process(gethome(), CHDIR);

    std::cout << "Do you want your vault to be hidden (i.e. not visible in your file manager)? [y/n]\n";
    std::cin >> Ui;

    if (Ui == 'y' || Ui == 'Y')
        if (!Fs::exists(hVAULT))
            Fs::create_directories(hVAULT);

    else
        if (!Fs::exists(vVAULT))
            Fs::create_directories(vVAULT);
}

void lConfigure()
{
    char FullCommand[MAX_LEN];

    std::string RepoPath;
    std::string Address;
    std::string User;

    int Port;

    if (!Fs::exists(CONFIG))
        Fs::create_directories(CONFIG);

    Process(CONFIG, CHDIR);

    std::ifstream ConfigFile(PREF);

    if (!ConfigFile.is_open())
    {
        std::ofstream wConfigFile(PREF);

        if (!wConfigFile.is_open())
            Throw(FSTREAM, 1);

        std::cout << "Please enter server settings\n";
        std::cout << "NOTE: Only SSH is supported!\n";
        std::cout << "Port: ";
        std::cin.CLR;
        std::cin >> Port;

        std::cout << "Username: ";
        std::cin.CLR;
        std::getline(std::cin, User);

        std::cout << "Hostname/address: ";
        std::cin.CLR;
        std::getline(std::cin, Address);

        std::cout << "Server repository path: ";
        std::cin.CLR;
        std::getline(std::cin, RepoPath);

        wConfigFile << Port;
        wConfigFile << User;
        wConfigFile << Address;
        wConfigFile << RepoPath;

        wConfigFile.close();
    }

    ConfigFile >> Port;
    ConfigFile >> User;
    ConfigFile >> Address;
    ConfigFile >> RepoPath;
    ConfigFile.close();

    if (!Fs::exists(TMP))
        Fs::create_directories(TMP);

    else if (Fs::exists(TMP))
    {
        Fs::remove_all(TMP);
        Fs::create_directories(TMP);
    }

    Process(TMP, CHDIR);

    snprintf(FullCommand, MAX_LEN, "git clone ssh://%s@%s:%d/%s .", User.c_str(), Address.c_str(), Port, RepoPath.c_str());

    if (std::system(FullCommand) != 0)
        Throw(CLONE, 1);

    const Fs::path Parent = Fs::path(TMP).parent_path();

    for (const auto& Entry : Fs::directory_iterator(TMP))
        Fs::rename(Entry, Parent / Entry.path().filename());

    Process("..", CHDIR);

    Fs::remove_all(TMP);
}

void gConfigure()
{
    std::string RepositoryName;
    char FullCommand[MAX_LEN];

    std::cin.CLR;
    std::getline(std::cin, RepositoryName);

    if (!Fs::exists(TMP))
        Fs::create_directories(TMP);

    else if (Fs::exists(TMP))
    {
        Fs::remove_all(TMP);
        Fs::create_directories(TMP);
    }

    Process(TMP, CHDIR);

    snprintf(FullCommand, MAX_LEN, "git clone %s", RepositoryName.c_str());

    if (std::system(FullCommand) != 0)
    {
        std::cerr << "Could not clone.\n";
        std::exit(1);
    }

    const Fs::path Parent = Fs::path(TMP).parent_path();

    for (const auto& Entry : Fs::directory_iterator(TMP))
        Fs::rename(Entry, Parent / Entry.path().filename());

    Process("..", CHDIR);

    Fs::remove_all(TMP);
}

void Client()
{
    int ExistType = 0;
    int RepoType = 0;

    std::cout << "Checking for vault location...\n";

    Process(gethome(), CHDIR);

    if (!Fs::exists(hVAULT))
        ExistType = 1;

    if (!Fs::exists(vVAULT))
        ExistType = 2;

    if (ExistType != 1 || ExistType != 2)
        MakeVault();

    std::cout << "Checking Git status...\n";

    Process("git", SYS);

    std::cout << "Checking for Git repository existence...\n";

    if (ExistType == 1)
        Process(hVAULT, CHDIR);

    else
        Process(vVAULT, CHDIR);

    if (!Fs::exists(GIT))
    {
        int Ui;

        std::cout << "Setting up repository...\n";
        std::cout << "Please select repository type:\n\n";
        std::cout << "1. Local repository\n";
        std::cout << "2. Github repository (*RECOMMENDED*)\n\n";
        std::cout << "> ";
        std::cin.CLR;
        std::cin >> Ui;

        if (Ui)
        {
            RepoType = 1;
            lConfigure();
        }

        else if (Ui == 2)
        {
            RepoType = 2;
            gConfigure();
        }
    }

    std::cout << "Making sure you are in the correct directory...\n";

    Process(gethome(), CHDIR);

    if (ExistType == 1)
        Process(hVAULT, CHDIR);

    else
        Process(vVAULT, CHDIR);

    std::cout << "Checking user...\n";

    if (std::system("git config user.email") != 0)
        std::system("git config user.email OSync@obsidian.md");

    if (std::system("git config user.name") != 0)
        std::system("git config user.name OSyncClient");

    Cloque(1);
}

void Server()
{}

int main(int argc, char* argv[])
{
    char Mode;

    std::cout << "Welcome to Obsidian Sync (v1.0)\n";
    std::cout << "(s) Server";
    std::cout << "(c) Client";

    if (strcmp(argv[1], "c") == 0)
        Client();

    else if (strcmp(argv[1], "s") == 0)
        Server();

    std::cout << "> ";
    std::cin >> Mode;

    if (Mode == C)
        Client();

    else if (Mode == S)
        Server();

    return 0;
}
