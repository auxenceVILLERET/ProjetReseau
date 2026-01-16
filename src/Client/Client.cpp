#include "pch.h"
#ifndef CLIENT_CPP_DEFINED
#define CLIENT_CPP_DEFINED

#include "Client.h"

#include <iostream>

#include "Message.h"
#include "Packets.hpp"

Client* Client::m_pInstance = nullptr;

Client::Client() : m_udpSocket()
{
    
}

Client::~Client()
{
    
}

Client* Client::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new Client();
        m_pInstance->Init();
    }
    return m_pInstance;
}

UDPSocket* Client::GetSocket()
{
    return &m_udpSocket;
}

void Client::Init()
{
    std::cout << "Please select your username : ";
    std::cin >> m_username;

    bool isConnected = false;
    while (!isConnected)
    {
        std::cout << "Please enter the server IP : ";
        std::cin >> m_serverIp;

        std::cout << "Please enter the server port : ";
        std::cin >> m_serverPort;
        
        Message msg;
        MessagePacket* packet = new MessagePacket("Connexion attempt");
        msg.AddPacket(packet);

        char* buffer = msg.Serialize();

        sockaddr_in target;
        if ( inet_pton(AF_INET, m_serverIp.c_str(), &target.sin_addr)<=0 )
            continue;
        target.sin_family = AF_INET;
        target.sin_port = htons(m_serverPort);

        if (m_udpSocket.SendTo(buffer, strlen(buffer) + 1, target) != SOCKET_ERROR)
        {
            isConnected = true;
        }
        else
        {
            std::cout << "Unvalid Server address, please try again" << std::endl;
        }
    }
}

bool Client::Connect(char* ip, int port)
{
    return false;
}


#endif
