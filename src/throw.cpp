#include <iostream>
#include <cerrno>

int ThrowError(char Msg[], int ErrCode)
{
    std::cerr << Msg << ": " << std::strerror(errno) << '\n';

    return ErrCode;
}
