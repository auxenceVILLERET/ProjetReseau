#ifndef MESSAGE_H_DEFINED
#define MESSAGE_H_DEFINED
#include <cstdint>
#include <sal.h>
#include <vector>

class Packet;

class Message
{
public:
    Message(bool isNew = false);
    ~Message();
    
    char* Serialize();
    std::vector<Packet*> Deserialize(char* message);
    bool AddPacket(Packet* packet);

    void ClearPackets();
    
    static constexpr uint32_t BUFFER_SIZE = 1024;
    
private:
    static uint32_t ID_COUNT;
    static constexpr uint32_t MAX_ID = 1000;
    static constexpr uint16_t MAGIC_WORD = 0b1010101111001101;
    
    uint32_t m_id;
    uint32_t m_size;
    int m_packetCount;
    bool m_isSystemMsg = false;

    std::vector<Packet*> m_vPackets;
};

#endif
