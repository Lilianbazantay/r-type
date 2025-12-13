#include "NetworkBuffer.hpp"

void NetworkBuffer::pushPacket(ServerPacket& pkt)
{
    std::lock_guard<std::mutex> lock(mtx);
    packets.push_back(pkt);
}

ServerPacket NetworkBuffer::popPacket()
{
    std::lock_guard<std::mutex> lock(mtx);

    ServerPacket out = packets.front();
    packets.erase(packets.begin());
    return out;
}

std::vector<ServerPacket> NetworkBuffer::popAllPackets()
{
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<ServerPacket> out = packets;
    packets.clear();
    return out;
}

bool NetworkBuffer::empty()
{
    std::lock_guard<std::mutex> lock(mtx);
    return packets.empty();
}
