#include "Message.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Packets/Packets.h"

uint32_t Message::ID_COUNT = 0;

Message::Message(bool isNew)
{
    if (isNew == false) return;

    ID_COUNT++;

    if (ID_COUNT > MAX_ID)
        ID_COUNT = MAX_ID;

    m_id = ID_COUNT;
    m_packetCount = 0;
    m_size = sizeof(MAGIC_WORD) + sizeof(m_id) + sizeof(m_packetCount) + sizeof(m_isSystemMsg);
}

Message::~Message()
{
    
}

void Message::Serialize(char* buffer)
{
    char* bufferCursor = buffer;
    
    std::memset(buffer, 0, BUFFER_SIZE);

    std::memcpy(bufferCursor, &MAGIC_WORD, sizeof(MAGIC_WORD));
    bufferCursor += sizeof(MAGIC_WORD);
    
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

    ClearPackets();
}

std::vector<Packet*> Message::Deserialize(char* message)
{
    std::vector<Packet*> result;
    if (!message)
        return result;

    uint16_t magicWord;
    std::memcpy(&magicWord, message, sizeof(MAGIC_WORD));
    message = message + sizeof(MAGIC_WORD);

    if (magicWord != MAGIC_WORD) return result;
    
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
            case PacketType::MESSAGE:
                packet = new MessagePacket();
                break;
            case PacketType::PING_PONG:
                packet = new PingPongPacket();
                break;
            case PacketType::CREATE_ENTITY:
                packet = new CreateEntity();
                break;
            case PacketType::SET_PLAYER_ID:
                packet = new SetPlayerIDPacket();
                break;
            case PacketType::SET_ENTITY_POS:
                packet = new SetEntityPos();
                break;
            case PacketType::SET_ENTITY_ROT:
                packet = new SetEntityRot();
                break;
            case PacketType::SET_ENTITY_SCALE:
                packet = new SetEntityScale();
                break;
            case PacketType::MOVE_ENTITY:
                packet = new MoveEntityPacket();
                break;
            case PacketType::ROTATE_ENTITY:
                packet = new RotateEntityPacket();
                break;
            case PacketType::SCALE_ENTITY:
                packet = new ScaleEntityPacket();
                break;
            case PacketType::SET_PLAYER_SPEED:
                packet = new SetPlayerSpeedPacket();
                break;
            case PacketType::CHANGE_PLAYER_SPEED:
                packet = new ChangePlayerSpeedPacket();
                break;
			case PacketType::SHOOT_PROJECTILE:
				packet = new ShootProjectilePacket();
				break;
            case PacketType::DESTROY_ENTITY:
                packet = new DestroyEntityPacket();
                break;
			case PacketType::SET_ACTIVE_STATE:
				packet = new SetActiveStatePacket();
				break;
            case PacketType::SET_HEALTH:
                packet = new SetEntityHealthPacket();
				break;
			case PacketType::SET_ENTITY_DIR:
				packet = new SetEntityDirPacket();
				break;
			case PacketType::CHAT_MESSAGE:
				packet = new ChatMessagePacket();
				break;
			case PacketType::CHANGE_COLOR_SHIP:
				packet = new ChangeColorShipPacket();
				break;
            case PacketType::CHANGE_COLOR_PARTICLE:
                packet = new ChangeColorParticlePacket();
				break;
			case PacketType::SET_POWER_UP_TYPE:
				packet = new SetPowerUpTypePacket();
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

    m_packetCount++;
    m_vPackets.push_back(packet);
    m_size = size;
    return true;
}

void Message::ClearPackets()
{
    for (Packet* pkt : m_vPackets)
    {
        delete pkt;
    }
    m_vPackets.clear();
}
