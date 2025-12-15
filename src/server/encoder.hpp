#pragma once

#include <cstdint>
#include <vector>

/**
 * @brief definition of the encoding action type section of the protocol
 *
 */
enum ActionTypeServer {
    ENTITY_CREATED = 0b0000,
    ENTITY_MOVED = 0b0001,
    ENTITY_DELETED = 0b0010,
    BEGIN_GAME = 0b0100,
    PACKAGE_NOT_RECEIVED = 0b1100,
    SERVER_SHUTDOWN = 0b1110,
    VALIDATION = 0b1111
};

/**
 * @brief static class used to encode packets with all sort of informations
 *
 */
class PacketEncoder {
private:
    /**
     * @brief encode a packet based on given datas
     *
     * @param packetID id of the packet to encode
     * @param payloadSize size of the body of the packet
     * @param actionType type of action
     * @param entityType type of entity (if you send any entities)
     * @param entityID id of the entity that is affected by the action
     * @param posX position X of the entity (if any entity or if position is needed)
     * @param posY position Y of the entity (if any entity or if position is needed)
     * @return std::vector<uint8_t> packet containing the information
     */
    static std::vector<uint8_t> encode(
        uint16_t packetID, uint8_t payloadSize, uint8_t actionType,
        uint8_t entityType = 0, uint16_t entityID = 0,
        uint16_t posX = 0, uint16_t posY = 0) {
            std::vector<uint8_t> out(9);

            uint32_t pid = packetID;
            out[0] = static_cast<uint8_t>((pid >> 8u) & 0xFFu);
            out[1] = static_cast<uint8_t>( packetID & 0xFFu);
            out[2] = static_cast<uint8_t>(((payloadSize & 0x0Fu) << 4u) |
                ( actionType & 0x0Fu));

            out[3] = entityType;
            uint32_t eid = entityID;
            out[4] = static_cast<uint8_t>((eid >> 8u) & 0xFFu);
            out[5] = static_cast<uint8_t>( entityID        & 0xFFu);

            posX &= 0x0FFFu;
            posY &= 0x0FFFu;
            uint32_t pos24 =
                ((uint32_t)posX << 12u) |
                ((uint32_t)posY);
            out[6] = static_cast<uint8_t>((pos24 >> 16u) & 0xFFu);
            out[7] = static_cast<uint8_t>((pos24 >> 8u)  & 0xFFu);
            out[8] = static_cast<uint8_t>( pos24         & 0xFFu);
            return out;
        }

public:
        /**
         * @brief helper functions that creates an entity
         *
         * @param packetID id of the packet
         * @param entityType type of the entity to be created
         * @param entityID id of the entity to be created
         * @param posX position X of the entity to be created
         * @param posY position Y of the entity to be created
         * @return std::vector<uint8_t> packet with the information
         */
    static std::vector<uint8_t> encodeCreate(
        uint16_t packetID,
        uint8_t entityType, uint16_t entityID,
        uint16_t posX, uint16_t posY) {
            return encode(packetID,
                6,
                ENTITY_CREATED,
                entityType,
                entityID,
                posX,
                posY);
        }

    /**
     * @brief helper functions to signal that an entity has moved
     *
     * @param packetID id of the packet
     * @param entityID id of the entity
     * @param entityType type of the entity
     * @param posX new position X of the entity
     * @param posY new position Y of the entity
     * @return std::vector<uint8_t> packet with the information
     */
    static std::vector<uint8_t> encodeMove(
        uint16_t packetID,
        uint8_t entityType, uint16_t entityID,
        uint16_t posX, uint16_t posY) {
            return encode(packetID,
                6,
                ENTITY_MOVED,
                entityType,
                entityID,
                posX,
                posY);
        }

    /**
     * @brief helper functions to signal that an entity has been deleted
     *
     * @param packetID id of the packet to be sent
     * @param entityID id of the deleted entity
     * @return std::vector<uint8_t> packet with the information
     */
    static std::vector<uint8_t> encodeDelete(
        uint16_t packetID,
        uint16_t entityType,
        uint8_t entityID) {
            return encode(packetID,
                2,
                ENTITY_DELETED, // entity deleted
                entityType,
                entityID,
                0, 0);
        }

    /**
     * @brief helper function to signal that a game is starting
     *
     * @param packetID id of the packet to be sent
     * @return std::vector<uint8_t> packet with the information
     */
    static std::vector<uint8_t> encodeStart(
        uint16_t packetID) {
            return encode(packetID,
            0,
            BEGIN_GAME);
        }

    /**
     * @brief function to tell that a package hasn't been received
     *
     * @param packetID id of the package to be sent
     * @return std::vector<uint8_t> packet with the information
     */
    static std::vector<uint8_t> encodeNotReceived(
        uint16_t packetID){
            return encode(packetID,
            0,
            PACKAGE_NOT_RECEIVED);
        }

    /**
     * @brief function to tell the client that the data has been received
     *
     * @param packetID id of the package to be sent
     * @return std::vector<uint8_t> packet with the information
     */
    static std::vector<uint8_t> encodeOK(
        uint16_t packetID){
            return encode(packetID,
            0,
            VALIDATION);
        }

    static std::vector<uint8_t> encodeSHUTDOWN(
        uint16_t packetID) {
            return encode(packetID,
        0,
        SERVER_SHUTDOWN);
        }
};
