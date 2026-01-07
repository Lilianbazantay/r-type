#include "NetworkServerBuffer.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>

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


NetworkContinuousBuffer::NetworkContinuousBuffer(int servId) {
    serverCurrentId = servId;
}

void NetworkContinuousBuffer::addEntity(int type, int id, std::vector<uint8_t> packet) {
    std::lock_guard<std::mutex> lock(mtx);

    if (findEntity({type, id}) != -1)
        return;
    entities.push_back({{type, id}, {packet, {}}});
    packets.push_back(packet);
}

void NetworkContinuousBuffer::moveEntity(int type, int id, std::vector<uint8_t> packet) {
    std::lock_guard<std::mutex> lock(mtx);

    int pos = findEntity({type, id});
    if (pos == -1)
        return;
    std::vector<uint8_t> movedPacket = entities[pos].second.second;
    if (!movedPacket.empty())
        for (size_t i = 0; i < packets.size(); i++)
            if (packets[i] == movedPacket) {
                packets.erase(packets.begin() + i);
                break;
            }
    packets.push_back(packet);
    entities[pos].second.second = packet;
}

void NetworkContinuousBuffer::deleteEntity(int type, int id) {
    std::lock_guard<std::mutex> lock(mtx);

    int pos = findEntity({type, id});
    if (pos == -1) {
        return;
    }
    std::vector<uint8_t> movedPacket = entities[pos].second.second;
    std::vector<uint8_t> createdPacket = entities[pos].second.first;
    bool isMoveDeleted = movedPacket.empty();
    bool isCreationDeleted = false;
    size_t packetsCount = packets.size();
    for (size_t i = 0; i < packetsCount; i++) {
        if (!isMoveDeleted && packets[i] == movedPacket) {
            isMoveDeleted = true;
            packets.erase(packets.begin() + i);
            if (isCreationDeleted)
                return;
            i--;
            continue;
        }
        if (!isCreationDeleted && packets[i] == createdPacket) {
            isCreationDeleted = true;
            packets.erase(packets.begin() + i);
            if (isMoveDeleted)
                return;
            i--;
        }
    }
}

int NetworkContinuousBuffer::findEntity(std::pair<int, int> typeId) {
    size_t entitySize = entities.size();

    for (size_t i = 0; i < entitySize; i++)
        if (entities[i].first == typeId)
            return i;
    return -1;
}

std::vector<std::vector<uint8_t>> NetworkContinuousBuffer::getAllPackets() {
    std::lock_guard<std::mutex> lock(mtx);
    return packets;
}

bool NetworkContinuousBuffer::empty() {
    std::lock_guard<std::mutex> lock(mtx);
    return packets.empty();
}

void NetworkContinuousBuffer::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    packets.clear();
    entities.clear();
}
