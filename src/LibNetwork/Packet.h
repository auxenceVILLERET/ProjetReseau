#ifndef PACKET_H_DEFINED
#define PACKET_H_DEFINED

class Packet
{
public:
    Packet();
    Packet(int size, int type);
    ~Packet() = default;

    virtual void Deserialize(char* message);
    virtual char* Serialize();
    
protected:
    int m_size;
    int m_type;

private:
    friend class Message;
};

#endif
