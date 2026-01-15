#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "../LibNetwork/Socket.h"
#include "../LibNetwork//UDPSocket.h"
#include "Server.h"

CRITICAL_SECTION criticalSection;

DWORD WINAPI ThreadProcedure(LPVOID lpParam)
{
    EnterCriticalSection(&criticalSection);
    
    char responseBuffer[1024 + 1];
    sockaddr_in target;
    while (true)
    {
        int response = Server::GetInstance()->GetSocket()->ReceiveFrom(responseBuffer, 1024, target);

        if (response >= 0)
        {
            Server::GetInstance()->LogUser(target);
            Server::GetInstance()->PrintMessage(responseBuffer, target);
            Server::GetInstance()->GlobalMsg(responseBuffer);
        }
    }

    LeaveCriticalSection(&criticalSection);

    return 1;
}

int main(int argc, char* argv[])
{
    InitializeCriticalSection(&criticalSection);

    if (!Sockets::Start())
    {
        std::cout << "Erreur initialisation : " << Sockets::GetError() << std::endl;
    }
    
    Server* server = Server::GetInstance();

    HANDLE thread = CreateThread(NULL, 0, ThreadProcedure, NULL, 0, NULL);

    WaitForSingleObject(thread, INFINITE);

    DeleteCriticalSection(&criticalSection);
    
    Sockets::Release();
    return 0;
}