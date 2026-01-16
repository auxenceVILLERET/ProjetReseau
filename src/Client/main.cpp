#include "pch.h"
#include <windows.h>
#include "main.h"

#include <iostream>

#include "Client.h"

#pragma comment(lib, "Ws2_32.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int cmdShow)
{
    if (!Sockets::Start())
    {
        std::cout << "Erreur initialisation : " << Sockets::GetError() << std::endl;
    }
    
    Client* client = Client::GetInstance();
    
    // AMIGA NTSC FULLSCREEN
    //CPU_RUN(320, 200, true, true);

    // AMIGA PAL FULLSCREEN
    //CPU_RUN(320, 256, true, true);

    // RETRO FULLSCREEN
    CPU_RUN(512, 256, true, true);

    // MODERN
    //CPU_RUN(1024, 576);

    // FULL HD (please use release)
    //CPU_RUN(1920, 1080);

    return 0;
}