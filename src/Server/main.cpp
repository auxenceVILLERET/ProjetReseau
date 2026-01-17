#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Packets.hpp"
#include "../LibNetwork/Socket.h"
#include "../LibNetwork//UDPSocket.h"
#include "Server.h"

int main(int argc, char* argv[])
{
    if (!Sockets::Start())
    {
        std::cout << "Erreur initialisation : " << Sockets::GetError() << std::endl;
    }
    
    Server* server = Server::GetInstance();

    while (true)
    {
        server->Update();
    }
    
    Sockets::Release();
    return 0;
}