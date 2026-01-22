#include "pch.h"
#ifndef CLIENT_CPP_DEFINED
#define CLIENT_CPP_DEFINED

#include "Client.h"

#include <iostream>

#include "ClientMethods.h"
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
    m_isConnected = false;
    m_hasPinged = false;
    m_isRunning = true;
    CreateThread(NULL, 0, ReceiveThread, this, 0, NULL);
}

void Client::Update()
{
    HandlePackets();

    for (Message msg : m_pendingMessages)
    {
        msg.Serialize(m_buffer);
        m_udpSocket.SendTo(m_buffer, 1025, m_serverAddr);
        msg.ClearPackets();
    }
    m_pendingMessages.clear();
}

void Client::SendPacket(Packet* packet)
{
    std::cout << "Registered packet of type " << PacketTypeNames[packet->GetType()] << std::endl;
    
    if (m_pendingMessages.size() == 0)
    {
        Message msg;
        m_pendingMessages.push_back(msg);
    }

    Message* lastMessage = &m_pendingMessages.back();
    if (lastMessage->AddPacket(packet) == false)
    {
        Message msg;
        msg.AddPacket(packet);
        m_pendingMessages.push_back(msg);
    }
}

bool Client::Connect(std::string ip, int port, std::string username)
{
    m_username = username;
    Message msg;
    PingPongPacket* packet = new PingPongPacket(m_username, true);
    msg.AddPacket(packet);

    msg.Serialize(m_buffer);
    m_serverIp = ip;
    m_serverPort = port;

    sockaddr_in target;
    if ( inet_pton(AF_INET, m_serverIp.c_str(), &target.sin_addr)<=0 )
    {
        std::cout << "Unvalid Client address, please try again" << std::endl;
        return false;
    }
    target.sin_family = AF_INET;
    target.sin_port = htons(m_serverPort);
    m_serverAddr = target;
        
    if (m_udpSocket.SendTo(m_buffer, 1025, m_serverAddr) != SOCKET_ERROR)
    {
        m_hasPinged = true;
        Sleep(1000);
        HandlePackets();
    }

    memset(m_buffer, 0, 1024);
    return true;
}

DWORD Client::ReceiveThread(LPVOID lpParam)
{
    Client* pInstance = static_cast<Client*>(lpParam);
    while (pInstance->m_isRunning)
    {
        if (pInstance->m_hasPinged == false && pInstance->m_isConnected == false)
            continue;
        
        char responseBuffer[1024 + 1];
        sockaddr_in target;
        int response = pInstance->m_udpSocket.ReceiveFrom(responseBuffer, 1025, target);
        
        if (response == -1)
        {
            int err = Sockets::GetError();
            std::cout << err << std::endl;

            if (err == WSAEINTR)
                break;
            
            continue;
        }

        // std::cout << "Received message ";
        
        Message msg;
        std::vector<Packet*> packets = msg.Deserialize(responseBuffer);

        // std::cout << "containing " << packets.size() << " packets" << std::endl;
        
        pInstance->m_packetProtection.Enter();
        for (int i = 0; i < packets.size(); i++)
        {
            // std::cout << "Registered packet of type : " << packets[i]->GetType() << std::endl;
            pInstance->m_packets.push_back(packets[i]);
        }
        pInstance->m_packetProtection.Leave();
    }
    
    return 1;
}

void Client::HandlePackets()
{
    m_packetProtection.Enter();
    for (Packet* packet : m_packets)
    {
        PacketType type = (PacketType)packet->GetType();

        std::cout << "Received Packet Type: " << PacketTypeNames[type] << std::endl;
        
        if (m_hasPinged && type == PING_PONG)
        {
            PingPongPacket* casted = dynamic_cast<PingPongPacket*>(packet);
            if (casted == nullptr) continue;

            if (casted->isPing == false && m_username == casted->username)
            {
                // if (casted->ip == m_serverIp && casted->port == m_serverPort)
                m_isConnected = true;
            }
        }
        if (type == CREATE_ENTITY)
        {
            CreateEntity* casted = dynamic_cast<CreateEntity*>(packet);
            if (casted == nullptr) continue;

            ClientMethods::CopyEntity(casted);
        }
        if (type == SET_PLAYER_ID)
        {
            SetPlayerIDPacket* casted = dynamic_cast<SetPlayerIDPacket*>(packet);
            if (casted == nullptr) continue;

            m_playerID = casted->id;
        }
    }

    for (int i = 0; i < m_packets.size(); i++)
    {
        delete m_packets[i];
    }
    
    m_packets.clear();
    m_packetProtection.Leave();
}


#endif
