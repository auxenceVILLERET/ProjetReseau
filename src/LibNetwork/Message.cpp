#include "Message.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Packet.h"
#include "Packets.hpp"


uint32_t Message::ID_COUNT = 0;

Message::Message(bool isNew)
{
    if (isNew == false) return;

    ID_COUNT++;

    if (ID_COUNT > MAX_ID)
        ID_COUNT = MAX_ID;

    m_id = ID_COUNT;

    m_size = sizeof(m_id) + sizeof(m_packetCount) + sizeof(m_isSystemMsg);
}

char* Message::Serialize()
{
    char* buffer = new char[BUFFER_SIZE];
    char* bufferCursor = buffer;
    
    std::memset(buffer, 0, BUFFER_SIZE);
    
    std::memcpy(bufferCursor, &m_id, sizeof(m_id));
    bufferCursor += sizeof(m_id);

    std::memcpy(bufferCursor, &m_isSystemMsg, sizeof(m_isSystemMsg));
    bufferCursor += sizeof(m_isSystemMsg);
    
    m_packetCount = m_vPackets.size();
    std::memcpy(bufferCursor, &m_packetCount, sizeof(m_packetCount));
    bufferCursor += sizeof(m_packetCount);

    for (int i = 0; i < m_vPackets.size(); i++)
    {
        char* packetBuffer = m_vPackets[i]->Serialize();
        std::memcpy(bufferCursor, packetBuffer, m_vPackets[i]->m_size);
        bufferCursor += m_vPackets[i]->m_size;
        delete[] packetBuffer;
    }

    return buffer;
}

std::vector<Packet*> Message::Deserialize(char* message)
{
    std::vector<Packet*> result;
    if (!message)
        return result;
    
    std::memcpy(&m_id, message, sizeof(m_id));
    message = message + sizeof(m_id);
    std::memcpy(&m_isSystemMsg, message, sizeof(m_isSystemMsg));
    message += sizeof(m_isSystemMsg);

    if (m_isSystemMsg) return result;

    std::memcpy(&m_packetCount, message, sizeof(m_packetCount));
    message += sizeof(m_packetCount);
    
    for (int i = 0; i < m_packetCount; i++)
    {
        PacketType packetType;
        std::memcpy(&packetType, message, sizeof(packetType));

        Packet* packet = nullptr;
        
        switch (packetType)
        {
            default:
                break;
            case PacketType::BALL_UPDATE:
                packet = new BallUpdatePacket();
                break;
            case PacketType::MESSAGE:
                packet = new MessagePacket();
                break;
        }

        if (packet == nullptr) continue;
        
        packet->Deserialize(message);
        result.push_back(packet);
        message += packet->m_size;
    }

    return result; 
}

bool Message::AddPacket(Packet* packet)
{
    uint32_t size = packet->m_size + m_size;
    if (size > BUFFER_SIZE)
        return false;

    m_vPackets.push_back(packet);
    return true;
}
