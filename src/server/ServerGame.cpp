#include "../src/ecs/relevant_data.hpp"
#include "../src/ecs/System/SystemList.hpp"
#include "ecs/Component/Direction.hpp"
#include "ecs/Component/EntitySpawner.hpp"
#include "ecs/Component/Position.hpp"
#include "ecs/Component/Cooldown.hpp"
#include "ecs/Entity/Entities.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/IComponent.hpp"
#include "ecs/System/CollisionSystem.hpp"
#include "ecs/System/ShootSystem.hpp"
#include "ecs/System/StrategySystem.hpp"
#include "protocol.hpp"
#include "server.hpp"
#include "server/NetworkServerBuffer.hpp"
#include "server/encoder.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <csignal>
#include <cstddef>
#include <iostream>
#include <memory>
#include <fstream>
#include <signal.h>

#include "ServerGame.hpp"

static nlohmann::json loadJsonFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ServerGame] Failed to open file: " << filename << std::endl;
        return nlohmann::json{};
    }

    nlohmann::json j;
    try {
        file >> j;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "[ServerGame] Failed to parse JSON in " << filename << ": " << e.what() << std::endl;
        return nlohmann::json{};
    }
    return j;
}

ServerGame::ServerGame(int port, NetworkServerBuffer *newRBuffer, NetworkClientBuffer* newSBuffer, NetworkContinuousBuffer *newCBuffer):
    systemList(),
    factory(),
    networkReceiveBuffer(newRBuffer),
    networkSendBuffer(newSBuffer),
    continuousBuffer(newCBuffer),
    networkServer(port,
    newRBuffer, newSBuffer, newCBuffer),
    waveManager(factory)
{
    data.bullet_count = 0;
    data.enemy_count = 0;
    clock.restart();
    Prevtime = clock.getElapsedTime();
    systemList.push_back(std::make_unique<ShootSystem>());
    systemList.push_back(std::make_unique<CollisionSystem>());
    systemList.push_back(std::make_unique<MovementSystem>());
    systemList.push_back(std::make_unique<StrategySystem>());
    networkServer.start();

    factory.registerComponentConstructors();

    std::cerr << "[DEBUG] Starting configuration\n";

        factory.registerPrototypes({
        {"Background", {"Background", loadJsonFromFile("./configuration/background/Background.json")}},
        {"Player", {"Player", loadJsonFromFile("./configuration/player/Player.json")}},
        {"Enemy", {"Enemy", loadJsonFromFile("./configuration/enemy/Enemy.json")}},
        {"Wall1", {"Wall1", loadJsonFromFile("./configuration/enemy/Wall1.json")}},
        {"Wall2", {"Wall2", loadJsonFromFile("./configuration/enemy/Wall2.json")}},
        {"Wall3", {"Wall3", loadJsonFromFile("./configuration/enemy/Wall3.json")}},
        {"Wall4", {"Wall4", loadJsonFromFile("./configuration/enemy/Wall4.json")}},
        {"PlayerBullet", {"PlayerBullet", loadJsonFromFile("./configuration/weapon/bullet/PlayerBullet.json")}},
        {"EnemyBullet", {"EnemyBullet", loadJsonFromFile("./configuration/weapon/bullet/EnemyBullet.json")}}
    });
    createEntity(ENTITY_ENEMY, 1, "Wall1");
    createEntity(ENTITY_ENEMY, 2, "Wall2");
    createEntity(ENTITY_ENEMY, 3, "Wall3");
    createEntity(ENTITY_ENEMY, 4, "Wall4");

    // ========== Option B : on passe un callback pour WaveManager ==========
    waveManager.setCreateEntityCallback(
        [this](int type, relevant_data_t* dataPtr){
            if (!dataPtr) return;

            int newId = 0;
            switch (type) {
                case ENTITY_ENEMY:
                    newId = dataPtr->enemy_count++;
                    break;
                case ENTITY_BULLET:
                    newId = dataPtr->bullet_count++;
                    break;
                default:
                    std::cerr << "[ERROR][ServerGame] Unsupported type in callback: " << type << "\n";
                    return;
            }

            if (!this->createEntity(type, newId, "Enemy")) {
                std::cerr << "[ERROR][ServerGame] Failed to create entity type=" << type << "\n";
                return;
            }
            std::cerr << "[DEBUG][WaveManager] Entity created via createEntity id=" << newId
                      << " type=" << type << "\n";
        }
    );

    std::cerr << "[DEBUG] Configuration finished\n";
}


/**
 * @brief add the packet about creation of the entity to client buffer and continuous buffer
 *
 * @param entity newly entity
 *
 */
void ServerGame::updateCreation(std::unique_ptr<IMediatorEntity> &entity) {
    auto* playerPos = dynamic_cast<Position*>(
        entity->FindComponent(ComponentType::POSITION));
    if (!playerPos)
        return;
    int type = entity->getType();
    int id = entity->getId();
    auto pkt = encoder.encodeCreate(
        networkServer.currentID,
        type, id,
        playerPos->GetPosition().first,
        playerPos->GetPosition().second
    );
    networkSendBuffer->pushPacket(pkt);
    continuousBuffer->addEntity(type, id, pkt);
}

/**
 * @brief add the packet about death of the entity to client buffer and continuous buffer, then erases the entity
 *
 * @param entity dead entity
 * @param pos entity position in Array
 *
 */
void ServerGame::updateDeath(std::unique_ptr<IMediatorEntity> &entity, size_t pos) {
    int type = entity->getType();
    int id = entity->getId();
    auto pkt = encoder.encodeDelete(
        networkServer.currentID, type, id);
    networkSendBuffer->pushPacket(pkt);
    continuousBuffer->deleteEntity(type, id);
    data.entityList.erase(data.entityList.begin() + pos);
}

/**
 * @brief add the packet about updates of the entity to client buffer and continuous buffer
 *
 * @param entity updated entity
 *
 */
void ServerGame::updateModifications(std::unique_ptr<IMediatorEntity> &entity) {
    Position *playerPos = dynamic_cast<Position*>(entity->FindComponent(ComponentType::POSITION));
    if (playerPos == nullptr)
        return;
    std::vector<uint8_t> pkt = encoder.encodeMove(networkServer.currentID, entity->getType(),
        entity->getId(), _serverTick , playerPos->GetPosition().first, playerPos->GetPosition().second);
    continuousBuffer->moveEntity(entity->getType(), entity->getId(), pkt);
    networkSendBuffer->pushPacket(pkt);
}


/**
 * @brief serverGame main update function. execute systems, and checks entities for updates
 */
void ServerGame::Update() {
    sf::Time Newtime = clock.getElapsedTime();
    data.runtime =
        (Newtime.asMicroseconds() - Prevtime.asMicroseconds()) / 1000000.;
    Prevtime = Newtime;
    for (size_t j = 0; j < data.entityList.size(); ) {
        auto& entity = data.entityList[j];
        for (auto& system : systemList)
            system->checkEntity(*entity, data);
        if (entity->justCreated()) {
            updateCreation(entity);
            ++j;
            continue;
        }
        if (!entity->is_Alive()) {
            updateDeath(entity, j);
            continue;
        }
        if (entity->hasChanged())
            updateModifications(entity);
        ++j;
    }
}


void ServerGame::Loop() {
    Running = true;
    Cooldown cooldown(1.0);
    Cooldown tmp(0.01);
    cooldown.LaunchCooldown();
    while(Running && !g_sigint) {
        if (tmp.CheckCooldown() == true) {
            Update();
            tmp.LaunchCooldown();
        }
        //std::cerr << "[DEBUG] Starting WaveManager\n";
        waveManager.computeEntities(&data);
        //std::cerr << "[DEBUG] Finishing WaveManger\n";
        //std::cerr << "[DEBUG] Starting parseNetworkPackets\n";
        parseNetworkPackets();
        //std::cerr << "[DEBUG] Finishing parseNetworkPackets\n";
    }
    Running = false;
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
bool ServerGame::createEntity(int entity_type, int personnal_id, std::string subType) {
    std::unique_ptr<IMediatorEntity> entity;

    switch (entity_type) {
        case ENTITY_BACKGROUND:
            entity = std::make_unique<Background>(factory);
            break;
        case ENTITY_PLAYER:
            std::cout << "> > > Player creating" << std::endl;
            entity = std::make_unique<Player>(factory);
            break;
        case ENTITY_ENEMY:
            if (subType == "Enemy") {
                entity = std::make_unique<Enemy>(factory);
                break;
            }
            if (subType == "Wall1") {
                entity = std::make_unique<Wall1>(factory);
                break;
            }
            if (subType == "Wall2") {
                entity = std::make_unique<Wall2>(factory);
                break;
            }
            if (subType == "Wall3") {
                entity = std::make_unique<Wall3>(factory);
                break;
            }
            if (subType == "Wall4") {
                entity = std::make_unique<Wall4>(factory);
                break;
            }
            break;
        case ENTITY_BULLET:
            entity = std::make_unique<PlayerBullet>(factory);
            break;
            return false;
        default:
            return false;
    }

    entity->setId(personnal_id);
    data.entityList.push_back(std::move(entity));
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
                createEntity(ENTITY_PLAYER, pkt.getPlayerId(), "");
                break;
            }
            case ActionType::PLAYER_CONNECT: {
                createEntity(ENTITY_PLAYER, pkt.getPlayerId(), "");
                break;
            }
            case ActionType::START_GAME: {
                createEntity(ENTITY_PLAYER, pkt.getPlayerId(), "");
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
