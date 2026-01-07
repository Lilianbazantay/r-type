#include "../src/ecs/relevant_data.hpp"
#include "../src/ecs/System/SystemList.hpp"
#include "ecs/Component/Direction.hpp"
#include "ecs/Component/EntitySpawner.hpp"
#include "ecs/Component/Position.hpp"
#include "ecs/Component/Cooldown.hpp"
#include "ecs/Entity/Entities.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/System/CollisionSystem.hpp"
#include "ecs/System/ShootSystem.hpp"
#include "protocol.hpp"
#include "server.hpp"
#include "server/NetworkServerBuffer.hpp"
#include "server/encoder.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <memory>
#include <unistd.h>

#include "ServerGame.hpp"


ServerGame::ServerGame(int port, NetworkServerBuffer *newRBuffer, NetworkClientBuffer* newSBuffer, NetworkContinuousBuffer *newCBuffer):
    networkReceiveBuffer(newRBuffer),
    networkSendBuffer(newSBuffer),
    continuousBuffer(newCBuffer),
    networkServer(port,
    newRBuffer, newSBuffer, newCBuffer)
{
    clock.restart();
    Prevtime = clock.getElapsedTime();
    systemList.push_back(std::make_unique<ShootSystem>());
    systemList.push_back(std::make_unique<CollisionSystem>());
    systemList.push_back(std::make_unique<MovementSystem>());
    networkServer.start();
}


void ServerGame::Update() {
    sf::Time Newtime = clock.getElapsedTime();
    data.runtime = (Newtime.asMicroseconds() - Prevtime.asMicroseconds()) / 1000000.;
    Prevtime = Newtime;

    size_t SListSize = systemList.size();
    size_t EListSize = data.entityList.size();
    for (size_t j = 0; j < EListSize; j++) {
        for (size_t i = 0; i < SListSize; i++)
            systemList[i]->checkEntity(*data.entityList[j].get(), data);
        if (data.entityList[j]->justCreated()) {
            Position *playerPos = dynamic_cast<Position*>(data.entityList[j]->FindComponent(ComponentType::POSITION));
            if (playerPos == nullptr)
                continue;
            std::vector<uint8_t> pkt = encoder.encodeCreate(networkServer.currentID,data.entityList[j]->getType(),
                data.entityList[j]->getId(), playerPos->GetPosition().first, playerPos->GetPosition().second);
            networkSendBuffer->pushPacket(pkt);
            std::cout << "Create entity" << std::endl;
            continuousBuffer->addEntity(data.entityList[j]->getType(), data.entityList[j]->getId(), pkt);
            continue;
        }
        if (!data.entityList[j]->is_Alive()) {
            std::vector<uint8_t> pkt = encoder.encodeDelete(networkServer.currentID, data.entityList[j]->getType(), data.entityList[j]->getId());
            networkSendBuffer->pushPacket(pkt);
            continuousBuffer->deleteEntity(data.entityList[j]->getType(), data.entityList[j]->getId());
            data.entityList.erase(data.entityList.begin() + j);
            j--;
            EListSize--;
           continue;
        }
        if (data.entityList[j]->hasChanged()) {
            Position *playerPos = dynamic_cast<Position*>(data.entityList[j]->FindComponent(ComponentType::POSITION));
            if (playerPos == nullptr)
                continue;
            std::vector<uint8_t> pkt = encoder.encodeMove(networkServer.currentID, data.entityList[j]->getType(),
                data.entityList[j]->getId(), playerPos->GetPosition().first, playerPos->GetPosition().second);
            continuousBuffer->moveEntity(data.entityList[j]->getType(), data.entityList[j]->getId(), pkt);
            networkSendBuffer->pushPacket(pkt);
            continue;
        }
        ++j;
    }
}


void ServerGame::Loop() {
    Running = true;
    Cooldown cooldown(20.0);
    Cooldown tmp(0.01);
    cooldown.LaunchCooldown();
    while(Running) {
        if (tmp.CheckCooldown() == true) {
            Update();
            tmp.LaunchCooldown();
        }
        if (cooldown.CheckCooldown() == true) {
            data.enemy_count++;
//            std::cout << "Set enemy" << std::endl;
            createEntity(2, data.enemy_count);
            cooldown.LaunchCooldown();
        }
        parseNetworkPackets();
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
        if (playerDirection == nullptr) {
            std::cout << "Error Null" << std::endl;
            return;
        }
        std::pair<float, float> direction = playerDirection->GetDirection();
        switch (newValues.second) {
            case 1: {
                direction.second += (1 * released);
                break;
            }
            case 2: {
                direction.second += (-1 * released);
                break;
            }
            case 3: {
                direction.first += (1 * released);
                break;
            }
            case 4: {
                direction.first += (-1 * released);
                break;
            }
            default:
                return;
        }
        playerDirection->SetDirection(direction);
        return;
    }
}

void ServerGame::playerShoot(int player_id) {
    for (size_t i = 0; i < data.entityList.size(); i++) {
        if (!data.entityList[i]->is_wanted_entity(ENTITY_PLAYER, player_id))
            continue;
        EntitySpawner *playerEntitySpawner = dynamic_cast<EntitySpawner*>(data.entityList[i]->FindComponent(ComponentType::ENTITY_SPAWNER));
        if (playerEntitySpawner == nullptr) {
            return;
        }
        playerEntitySpawner->enableSpawn();
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
    auto packets = networkReceiveBuffer->popAllPackets();
    for (auto& pkt : packets) {
        switch (pkt.getActionType()) {
            case ActionType::INPUT_PRESSED: {
                if (pkt.getActionvalue() == 0) {
                    playerShoot(pkt.getPlayerId());
                } else
                    changePlayerDirection(pkt.getPlayerId(), {ActionType::INPUT_PRESSED, pkt.getActionvalue()});
                break;
            }
            case ActionType::INPUT_RELEASED: {
                changePlayerDirection(pkt.getPlayerId(), {ActionType::INPUT_RELEASED, pkt.getActionvalue()});
                break;
            }
            case ActionType::NEW_CONNECTION: {
                createEntity(ENTITY_PLAYER, pkt.getPlayerId());
                break;
            }
            case ActionType::PLAYER_CONNECT: {
                createEntity(ENTITY_PLAYER, pkt.getPlayerId());
                break;
            }
            case ActionType::START_GAME: {
                createEntity(ENTITY_PLAYER, pkt.getPlayerId());
                Running = true;
                continue;
//                std::vector<uint8_t> start_pkt = encoder.encodeStart(0);
//                networkSendBuffer->pushPacket(start_pkt);
//                break;
            }
            case ActionTypeServer::SERVER_SHUTDOWN: {
                Running = false;
                break;
            }
            default:
                continue;
        }
    }
}
