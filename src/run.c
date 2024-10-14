#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_INPT_LEN 100

void ClearBuffer(void)
{
    int Char;
    while ((Char = getchar()) != '\n' && Char != EOF);
}

void Server(void)
{}

void Client(void)
{
    char Inpt[MAX_INPT_LEN];

    printf("> ");
    scanf("%99s", Inpt);
    ClearBuffer();

    if (!strcmp(Inpt, "fetch"))
    {
        
    }
}

void Run(char* Mode)
{
    if (Mode == NULL) Client();
    else Server();
}
