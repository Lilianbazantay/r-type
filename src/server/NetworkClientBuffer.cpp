#include "NetworkServerBuffer.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>

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
 * @brief add multiple packets to the buffer
 *
 * @param pkt packet to be added
 */
void NetworkClientBuffer::pushWholePacket(std::vector<std::vector<uint8_t>>& vect_pkt)
{
    std::lock_guard<std::mutex> lock(mtx);
    for (size_t i = 0; i < vect_pkt.size(); i++) {
        packets.push_back(vect_pkt[i]);
    }
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
    entities.clear();
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

/**
 * @brief move a specific entity. Updates a precendent package made for the same entity
 *
 * @param entityType Type of the entity
 * @param entityId ID of the entity
 * @param message message
 */
void NetworkClientBuffer::moveEntity(int entityType, int entityId, std::vector<uint8_t>& message) {
    std::lock_guard<std::mutex> lock(mtx);

    int entityPos = findEntity({entityType, entityId});
    if (entityPos == -1) {
        entities.push_back({{entityType, entityId}, packets.size()});
        packets.push_back(message);
    } else {
        packets[entities[entityPos].second] = message;
    }
}

/**
 * @brief finds a specific entity
 *
 * @param typeId Identifiers of the entity
 *
 * @return -1 if the entity does not exist in vector
 * @return entity position in vector
 */
int NetworkClientBuffer::findEntity(std::pair<int, int> typeId) {
    size_t entitySize = entities.size();

    for (size_t i = 0; i < entitySize; i++)
        if (entities[i].first == typeId)
            return i;
    return -1;
}