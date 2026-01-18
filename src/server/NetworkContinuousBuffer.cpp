#include "NetworkServerBuffer.hpp"
#include <cstddef>
#include <cstdint>

NetworkContinuousBuffer::NetworkContinuousBuffer(int servId) {
    serverCurrentId = servId;
}

/**
 * @brief creates a packet for entity creation.
 *
 * @param type entity type
 * @param id entity id
 * @param packets message
*/
void NetworkContinuousBuffer::addEntity(int type, int id, std::vector<uint8_t> packet) {
    std::lock_guard<std::mutex> lock(mtx);

    if (findEntity({type, id}) != -1)
        return;
    entities.push_back({{type, id}, {packet, {}}});
    packets.push_back(packet);
}

/**
 * @brief creates a packet for entity movement. Replaces packet if entity had already moved.
 *
 * @param type entity type
 * @param id entity id
 * @param packets message
*/
void NetworkContinuousBuffer::moveEntity(int type, int id, std::vector<uint8_t> packet) {
    std::lock_guard<std::mutex> lock(mtx);

    int pos = findEntity({type, id});
    if (pos == -1)
        return;
    std::vector<uint8_t> movedPacket = entities[pos].second.second;
    if (!movedPacket.empty()) {
        auto it = std::ranges::find(packets.begin(), packets.end(), movedPacket);
        if (it != packets.end())
            packets.erase(it);
    }
    packets.push_back(packet);
    entities[pos].second.second = packet;
}

/**
 * @brief deletes packets related to a specific entity
 *
 * @param type entity type
 * @param id entity id
*/
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
    for (size_t i = 0; i < packets.size(); ) {
        if (!isMoveDeleted && packets[i] == movedPacket) {
            isMoveDeleted = true;
            packets.erase(packets.begin() + i);
            if (isCreationDeleted)
                break;
            continue;
        }
        if (!isCreationDeleted && packets[i] == createdPacket) {
            isCreationDeleted = true;
            packets.erase(packets.begin() + i);
            if (isMoveDeleted)
                break;
            continue;
        }
        ++i;
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
int NetworkContinuousBuffer::findEntity(std::pair<int, int> typeId) {
    size_t entitySize = entities.size();

    for (size_t i = 0; i < entitySize; i++)
        if (entities[i].first == typeId)
            return i;
    return -1;
}

/**
 * @brief getter for every packet. This replaces popAll as continuous must keep track of every entity
 *
 * @return packet array
*/
std::vector<std::vector<uint8_t>> NetworkContinuousBuffer::getAllPackets() {
    std::lock_guard<std::mutex> lock(mtx);
    return packets;
}

/**
 * @brief checks if packets vector is empty
 *
 * @return true if vector is empty
 * @return false if vector is not empty
*/
bool NetworkContinuousBuffer::empty() {
    std::lock_guard<std::mutex> lock(mtx);
    return packets.empty();
}

/**
 * @brief empty the packets and entities vector
*/
void NetworkContinuousBuffer::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    packets.clear();
    entities.clear();
}
