#include "NetworkBuffer.hpp"

/**
 * @brief add a packet to the buffer
 *
 * @param pkt packet to be added
 */
void NetworkBuffer::pushPacket(ServerPacket& pkt)
{
    std::lock_guard<std::mutex> lock(mtx);
    packets.push_back(pkt);
}

/**
 * @brief remove a packet from a buffer
 *
 * @return packet that was removed
 */
ServerPacket NetworkBuffer::popPacket()
{
    std::lock_guard<std::mutex> lock(mtx);

    ServerPacket out = packets.front();
    packets.erase(packets.begin());
    return out;
}

/**
 * @brief remove all packet from buffer
 *
 * @return std::vector<ServerPacket> list of packet
 */
std::vector<ServerPacket> NetworkBuffer::popAllPackets()
{
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<ServerPacket> out = packets;
    packets.clear();
    return out;
}

/**
 * @brief tell if the buffer is empty
 *
 * @return true buffer is empty
 * @return false buffer is not empty
 */
bool NetworkBuffer::empty()
{
    std::lock_guard<std::mutex> lock(mtx);
    return packets.empty();
}
