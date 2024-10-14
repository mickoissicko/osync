#include "../include/stddef.h"

#include <iostream>
#include <unistd.h>

int ChangeDir(char Dir[], int UseHardMode)
{
    if (chdir(Dir) != 0)
    {
        if (UseHardMode) std::exit((ThrowError("Error whilst changing dir", OPN_DIR)));

        else return (ThrowError("Error whilst changing dir", OPN_DIR));
    }

    else return DEF_OK;
}

void Client(char StartingDir[])
{
    ChangeDir(StartingDir, 1);

    // Host (e.g. 192.168.x.x)
    // Port (e.g. 80, 443, 8080)
    // Username
    // Passphrase/password
    // Private key dir
}

void Server(char StartingDir[])
{
    ChangeDir(StartingDir, 1)

    // Port (e.g. 80, 443, 8080)
    // Host (e.g. 0.0.0.0, 127.0.0.1, localhost)
}
