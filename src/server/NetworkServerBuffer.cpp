#include "NetworkServerBuffer.hpp"

/**
 * @brief add a packet to the buffer
 *
 * @param pkt packet to be added
 */
void NetworkServerBuffer::pushPacket(ServerPacket& pkt)
{
    std::lock_guard<std::mutex> lock(mtx);
    packets.push_back(pkt);
}

/**
 * @brief remove a packet from a buffer
 *
 * @return packet that was removed
 */
ServerPacket NetworkServerBuffer::popPacket()
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
std::vector<ServerPacket> NetworkServerBuffer::popAllPackets()
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
bool NetworkServerBuffer::empty()
{
    std::lock_guard<std::mutex> lock(mtx);
    return packets.empty();
}


/**
 * @brief add a packet to the buffer
 *
 * @param pkt packet to be added
 */
void NetworkClientBuffer::pushPacket(std::vector<uint8_t>& pkt)
{
    std::lock_guard<std::mutex> lock(mtx);
    packets.push_back(pkt);
}

/**
 * @brief remove a packet from a buffer
 *
 * @return packet that was removed
 */
std::vector<uint8_t> NetworkClientBuffer::popPacket()
{
    std::lock_guard<std::mutex> lock(mtx);

    std::vector<uint8_t> out = packets.front();
    packets.erase(packets.begin());
    return out;
}

/**
 * @brief remove all packet from buffer
 *
 * @return std::vector<std::vector<uint8_t>> list of packet
 */
std::vector<std::vector<uint8_t>> NetworkClientBuffer::popAllPackets()
{
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::vector<uint8_t>> out = packets;
    packets.clear();
    return out;
}

/**
 * @brief tell if the buffer is empty
 *
 * @return true buffer is empty
 * @return false buffer is not empty
 */
bool NetworkClientBuffer::empty()
{
    std::lock_guard<std::mutex> lock(mtx);
    return packets.empty();
}
