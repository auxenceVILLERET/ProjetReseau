#ifndef SET_PLAYER_USERNAME_PACKET_HPP_DEFINED
#define SET_PLAYER_USERNAME_PACKET_HPP_DEFINED

#include "Packet.h"

class SetPlayerUsernamePacket : public Packet
{
public:
    uint32_t id;
    std::string username;
    size_t len;
    
    SetPlayerUsernamePacket()
    {
        id = 0;
        len = 0;

        m_type = SET_PLAYER_USERNAME;
        m_size = 2 * sizeof(int) + sizeof(id) + len + sizeof(len);
    }
    SetPlayerUsernamePacket(uint32_t _id, std::string _username) : username(_username)
    {
        id = _id;
        len = username.length() + 1;

        m_type = SET_PLAYER_USERNAME;
        m_size = 2 * sizeof(int) + sizeof(id) + len + sizeof(len);
    }

    char* Serialize()
    {
        char* buffer = new char[m_size];
        char* bufferCursor = buffer;
        std::memset(bufferCursor, 0, m_size);

        std::memcpy(bufferCursor, &m_type, sizeof(m_type));
        bufferCursor += sizeof(m_type);
        std::memcpy(bufferCursor, &m_size, sizeof(m_size));
        bufferCursor += sizeof(m_size);

        std::memcpy(bufferCursor, &id, sizeof(id));
        bufferCursor += sizeof(id);
        
        std::memcpy(bufferCursor, &len, sizeof(len));
        bufferCursor += sizeof(len);
        
        std::memcpy(bufferCursor, username.c_str(), len);
        bufferCursor += len;

        return buffer;
    }

    void Deserialize(char* _message)
    {
        Packet::Deserialize(_message);
        _message += sizeof(m_type) + sizeof(m_size);

        std::memcpy(&id, _message, sizeof(id));
        _message += sizeof(id);
        
        std::memcpy(&len, _message, sizeof(len));
        _message += sizeof(len);
        
        username.assign(_message, len - 1);
        _message += len;
    }
};

#endif