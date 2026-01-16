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
    CreateThread(NULL, 0, ReceiveThread, this, 0, NULL);
    std::cout << "Please select your username : ";
    std::cin >> m_username;

    while (!m_isConnected)
    {
        std::cout << "Please enter the server IP : ";
        std::cin >> m_serverIp;

        std::cout << "Please enter the server port : ";
        std::cin >> m_serverPort;
        
        Message msg;
        PingPongPacket* packet = new PingPongPacket(m_serverIp, m_username, m_serverPort, true);
        msg.AddPacket(packet);

        char* buffer = msg.Serialize();

        sockaddr_in target;
        if ( inet_pton(AF_INET, m_serverIp.c_str(), &target.sin_addr)<=0 )
            continue;
        target.sin_family = AF_INET;
        target.sin_port = htons(m_serverPort);
        
        if (m_udpSocket.SendTo(buffer, strlen(buffer) + 1, target) != SOCKET_ERROR)
        {
            m_hasPinged = true;
            Sleep(500);
        }
        
        if (m_isConnected)
        {
            m_hasPinged = false;
            std::cout << "Connection Successful" << std::endl;
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

DWORD Client::ReceiveThread(LPVOID lpParam)
{
    Client* pInstance = static_cast<Client*>(lpParam);
    while (true)
    {
        char responseBuffer[1024 + 1];
        sockaddr_in target;
        int response = pInstance->m_udpSocket.ReceiveFrom(responseBuffer, 1024, target);

        Message msg;
        std::vector<Packet*> packets = msg.Deserialize(responseBuffer);

        for (int i = 0; i < packets.size(); i++)
        {
            pInstance->m_packets.push_back(packets[i]);
        }
    }
    
    return 1;
}


void Client::HandlePackets()
{
    for (Packet* packet : m_packets)
    {
        PacketType type = (PacketType)packet->GetType();

        if (m_hasPinged && type == PING_PONG)
        {
            PingPongPacket* casted = dynamic_cast<PingPongPacket*>(packet);
            if (casted == nullptr) continue;

            if (casted->isPing == false)
            {
                if (casted->ip == m_serverIp && casted->port == m_serverPort)
                    m_isConnected = true;
            }
        }
    }

    for (int i = 0; i < m_packets.size(); i++)
    {
        delete m_packets[i];
    }
    
    m_packets.clear();
}


#endif
