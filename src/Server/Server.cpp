#ifndef SERVER_CPP_DEFINED
#define SERVER_CPP_DEFINED

#include "Server.h"

#include <iostream>
#include <WS2tcpip.h>

#include "Message.h"
#include "Packets.hpp"
#include "../LibNetwork/Socket.h"

Server* Server::m_pInstance = nullptr;

Server::Server() : m_udpSocket()
{
    
}

Server::~Server()
{
    
}

Server* Server::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new Server();
        m_pInstance->Init();
    }
    return m_pInstance;
}

void Server::Init()
{
    if (!m_udpSocket.Bind("127.0.0.1", 1888))
    {
        std::cout << "Connexion attempt failed.\n" << Sockets::GetError() << "\n";
    }
    std::cout << "Connection established.\n";
}

UDPSocket* Server::GetSocket()
{
    return &m_udpSocket;
}

void Server::LogUser(sockaddr_in newAddr)
{
    bool exists = false;
    char ip[23];
    inet_ntop(AF_INET, &newAddr.sin_addr, ip, INET_ADDRSTRLEN);
    int port = ntohs(newAddr.sin_port);

    for (sockaddr_in addr : m_vClients)
    {
        char ipTemp[23];
        inet_ntop(AF_INET, &addr.sin_addr, ipTemp, INET_ADDRSTRLEN);
        int portTemp = ntohs(addr.sin_port);

        if (strcmp(ip, ipTemp) == 0 && portTemp == port)
            exists = true;
    }

    if (exists) return;

    m_vClients.push_back(newAddr);
}

void Server::GlobalMsg(const char* msg)
{
    for (sockaddr_in addr : m_vClients)
    {
        char ip[23];
        inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
        int port = ntohs(addr.sin_port);

        std::string msgTemp = "[";
        msgTemp.append(ip);
        msgTemp.append(" : ");
        msgTemp.append(std::to_string(port));
        msgTemp.append("] : ");
        msgTemp.append(msg);
        msgTemp.append("\n");
        
        m_udpSocket.SendTo(msgTemp.c_str(), msgTemp.size() + 1, addr);
    }
}

void Server::PrintMessage(const char* msg, sockaddr_in addr)
{
    char ip[23];
    inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
    int port = ntohs(addr.sin_port);

    std::cout << "[" << ip << " : " << port << "] : " << msg << "\n";
}

DWORD Server::ReceiveThread(LPVOID lpParam)
{
    Server* pInstance = static_cast<Server*>(lpParam);
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

void Server::HandlePackets()
{
    for (Packet* packet : m_packets)
    {
        PacketType type = (PacketType)packet->GetType();

        if (type == PING_PONG)
        {
            PingPongPacket* casted = dynamic_cast<PingPongPacket*>(packet);
            if (casted == nullptr) continue;

            
        }
    }

    for (int i = 0; i < m_packets.size(); i++)
    {
        delete m_packets[i];
    }
    
    m_packets.clear();
}

void Server::SendPacket(Packet* packet)
{
    if (m_pendingMessages.size() == 0)
    {
        Message msg;
        m_pendingMessages.push_back(msg);
    }

    Message* lastMessage = &m_pendingMessages.back();
    if (lastMessage->AddPacket(packet) == false)
    {
        
    }
}


#endif
