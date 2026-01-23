#ifndef PACKET_CPP_DEFINED
#define PACKET_CPP_DEFINED

#include "Packet.h"

#include <cstring>
#include <iostream>
#include <ostream>

Packet::Packet()
{
    m_type = 0;
    m_size = 2 * sizeof(int);
}

Packet::Packet(int size, int type)
{
    m_type = type;
    m_size = size;
}

void Packet::Deserialize(char* message)
{
    std::memcpy(&m_type, message, sizeof(m_type));
    message = message + sizeof(m_type);
    std::memcpy(&m_size, message, sizeof(m_size));
    message = message + sizeof(m_size);
}

char* Packet::Serialize()
{
    char* buffer = new char[m_size];
    char* bufferCursor = buffer;
    std::memset(bufferCursor, 0, m_type);
    bufferCursor += sizeof(m_type);
    std::memcpy(bufferCursor, &m_type, sizeof(m_type));
    bufferCursor += sizeof(m_type);
    
    return bufferCursor;
}

void Packet::PrintInfo(bool isSent)
{
    std::cout << "[" << PacketTypeNames[m_type] << "] ";
    if (isSent)
        std::cout << "SENT";
    else
        std::cout << "RECEIVED";
    std::cout << std::endl;
}

#endif
