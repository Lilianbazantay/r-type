#include "../src/ecs/relevant_data.hpp"
#include "../src/ecs/System/SystemList.hpp"
#include "client/NetworkBuffer.hpp"
#include "ecs/Component/Direction.hpp"
#include "ecs/Entity/Entities.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/System/CollisionSystem.hpp"
#include "ecs/System/ShootSystem.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <memory>

#include "ServerGame.hpp"


ServerGame::ServerGame(int port, NetworkBuffer *newBuffer): networkBuffer(newBuffer), networkServer(port, newBuffer) {
    clock.restart();
    Prevtime = clock.getElapsedTime();
    systemList.push_back(std::make_unique<ShootSystem>());
    systemList.push_back(std::make_unique<CollisionSystem>());
    systemList.push_back(std::make_unique<MovementSystem>());
    createEntity(ENTITY_BACKGROUND, 0);
}


void ServerGame::Update() {
    sf::Time Newtime = clock.getElapsedTime();
    data.runtime = (Newtime.asMicroseconds() - Prevtime.asMicroseconds()) / 1000000.;
    Prevtime = Newtime;

    size_t SListSize = systemList.size();
    size_t EListSize = data.entityList.size();
    std::cout << "Sizes: " << SListSize << ", " << EListSize << std::endl;
    for (size_t i = 0; i < SListSize; i++)
        for (size_t j = 0; j < EListSize; j++)
            systemList[i]->checkEntity(*data.entityList[j].get(), data);
}

void ServerGame::Loop() {
    Running = true;
    while(1) {
        if (Running)
            Update();
    }
}

/**
 * @brief moves a specific player
 *
 * @param personnal_id id of the moving player
 * @param newDir new direction to assign
 *
 */
void ServerGame::changePlayerDirection(int personnal_id, std::pair<int, int> newValues) {
    int released = 1;
    if (newValues.first == 1) {
        released = -1;
    }
    for (size_t i = 0; i < data.entityList.size(); i++) {
        if (!data.entityList[i]->is_wanted_entity(ENTITY_PLAYER, personnal_id))
            continue;
        Direction *playerDirection = dynamic_cast<Direction*>(data.entityList[i]->FindComponent(ComponentType::DIRECTION));
        if (playerDirection == nullptr)
            return;
        std::pair<float, float> direction = playerDirection->GetDirection();
        switch (newValues.second) {
            case 1: {
                direction.first += (1 * released);
                break;
            }
            case 2: {
                direction.first += (-1 * released);
                break;
            }
            case 3: {
                direction.second += (1 * released);
                break;
            }
            case 4: {
                direction.second += (-1 * released);
                break;
            }
            default:
                return;
        }
        playerDirection->SetDirection(direction);
        return;
    }
}

/**
 * @brief creates an entity in the server array, based on the type entered
 *
 * @param entity_type type of the entity
 * @param personnal_id id of the new entity
 *
 * @return true if the entity is successfully created
 * @return false if the entity type was not found
 */
bool ServerGame::createEntity(int entity_type, int personnal_id) {
    switch (entity_type) {
        case ENTITY_BACKGROUND: {
            data.entityList.push_back(std::make_unique<Background>());
            break;
        }
        case ENTITY_PLAYER: {
            data.entityList.push_back(std::make_unique<Player>());
            break;
        }
        case ENTITY_ENEMY: {
            data.entityList.push_back(std::make_unique<Enemy>());
            break;
        }
        case ENTITY_BULLET: {
            data.entityList.push_back(std::make_unique<PlayerBullet>());
            break;
        }
        default:
            return false;
    }
    data.entityList[data.entityList.size() -1]->setId(personnal_id);
    return true;
}

void ServerGame::parseNetworkPackets() {
    auto packets = networkBuffer->popAllPackets();
    for (const auto& pkt : packets) {
        // PUT ENDPOINT ECS
        std::cout << "[GAME] Received packet id=" << pkt.packetId
                  << " playerId=" << pkt.entityId
                  << " action=" << (int)pkt.actionType
                  << " button=" << (int)pkt.entityId << std::endl;
    }
}