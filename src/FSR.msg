import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;
import inet.common.packet.Packet;
import inet.networklayer.common.L3Address;

namespace inet;

enum FSRControlPacketType
{
    UPDATE = 1;
};

class FSRControlPacket extends FieldsChunk
{
    FSRControlPacketType packetType = static_cast<FSRControlPacketType>(-1);
}

class FSRUpdatePacket extends FSRControlPacket
{
    std::unordered_map<std::string, std::unordered_set<std::string>> neighbors;
}
