#pragma once
#define _SOCKET int
#define INVALID_SOCKET ((int)-1)

namespace Sockets
{
    bool Start();
    void Release();
    int GetError();
    void CloseSocket(_SOCKET socket);
}
