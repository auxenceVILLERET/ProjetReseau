#ifndef PACKET_H_DEFINED
#define PACKET_H_DEFINED
#include <string>
#include <vector>

inline std::vector<std::string> PacketTypeNames
{
    "BALL UPDATE",
    "MESSAGE",
    "PING PONG",
    "CREATE ENTITY",
    "SET PLAYER ID",
    "SET ENTITY POS",
    "SET ENTITY ROT",
    "SET ENTITY SCALE",
    "MOVE ENTITY",
    "ROTATE ENTITY",
    "SCALE ENTITY"
};

class Packet
{
public:
    Packet();
    Packet(int size, int type);
    virtual ~Packet() = default;

    int GetType() { return m_type; }
    
    virtual void Deserialize(char* message);
    virtual char* Serialize();

    virtual void PrintInfo();
    
protected:
    int m_size;
    int m_type;

private:
    friend class Message;
};

#endif
