#include "ClientGame.hpp"
#include "../src/ecs/relevant_data.hpp"
#include "ecs/Component/Position.hpp"
#include "ecs/Entity/Entities.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/System/DrawSpriteSystem.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <memory>

/**
 * @brief Constructor for ClientGame. Init the window with a nice starfield
 *
 */
ClientGame::ClientGame(std::string ip, int port, NetworkBuffer *netBuffer): client(ip, port, netBuffer)
{
    _netBuffer = netBuffer;
    data.window.create(sf::VideoMode({1920, 1080}), "RTYPE");
    data.window.clear(sf::Color::Black);
    data.window.setActive(true);
    clock.restart();
    _inputManager.setClient(&client);
    Prevtime = clock.getElapsedTime();
    systemList.push_back(std::make_unique<DrawSpriteSystem>());
    createEntity(ENTITY_BACKGROUND, 0, {0, 0});
    data.window.setKeyRepeatEnabled(false);
    client.start();
}

/**
 * @brief updates the deltatime(runtime), and go through every entity and system.
 *
 */
void ClientGame::Update() {
    sf::Time Newtime = clock.getElapsedTime();
    data.runtime = (Newtime.asMicroseconds() - Prevtime.asMicroseconds()) / 1000000.;
    Prevtime = Newtime;

    size_t SListSize = systemList.size();
    size_t EListSize = data.entityList.size();
    for (size_t i = 0; i < SListSize; i++)
        for (size_t j = 0; j < EListSize; j++)
            systemList[i]->checkEntity(*data.entityList[j].get(), data);
}

/**
 * @brief main loop for ClientGame. clear window, display, and retrieves events. Does not update/draw the entities if paused
 *
 */
void ClientGame::Loop() {
    while(data.window.isOpen()) {
        sf::Event evt;
        if (Stopping)
            return;
        data.window.clear(sf::Color::Black);
        if (!Paused) {
            Update();
            processNetworkPackets();
        }
        data.window.display();
        while(data.window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed)
                data.window.close();
            _inputManager.processEvent(evt);
        }
    }
}

/**
 * @brief creates an entity based on its entity type, and assigns it an ID and a position
 *
 * @param entity_type the entity type to create. If not found, does not creates and entity
 * @param entity_id id allocated by the server for the entity
 * @param position position of the entity. Does nothing if the entity does not have the component POSITION
 */
void ClientGame::createEntity(int entity_type, int entity_id, std::pair<float, float> position) {
    int prevSize = data.entityList.size();
    switch (entity_type) {
        case ENTITY_BACKGROUND: {
            data.entityList.push_back(std::make_unique<Background>());
            break;
        } case ENTITY_PLAYER: {
            data.entityList.push_back(std::make_unique<Player>());
            break;
        } case ENTITY_ENEMY: {
            data.entityList.push_back(std::make_unique<Enemy>());
            break;
        } case ENTITY_BULLET: {
            data.entityList.push_back(std::make_unique<PlayerBullet>());
            break;
        } default:
            return;
    }
    data.entityList[prevSize]->setId(entity_id);
    Position *playerPosition = dynamic_cast<Position*>(data.entityList[prevSize]->FindComponent(ComponentType::POSITION));
    if (playerPosition != nullptr)
        playerPosition->SetPosition(position);
}

/**
 * @brief changes the entity position
 *
 * @param entity_type entity type
 * @param entity_id id allocated by the server for the entity
 * @param new_position new position of the entity. Does nothing if the entity does not have the component POSITION
 */

void ClientGame::moveEntity(int entity_type, int entity_id, std::pair<float, float> new_position) {
    for (size_t i = 0; i < data.entityList.size(); i++) {
        if (!data.entityList[i]->is_wanted_entity(entity_type, entity_id))
            continue;
        Position *playerPosition = dynamic_cast<Position*>(data.entityList[i]->FindComponent(ComponentType::POSITION));
        if (playerPosition != nullptr)
            playerPosition->SetPosition(new_position);
        return;
    }
}

/**
 * @brief deletes the specified entity
 *
 * @param entity_type entity type
 * @param entity_id id allocated by the server for the entity
 */
void ClientGame::deleteEntity(int entity_type, int entity_id) {
    for (size_t i = 0; i < data.entityList.size(); i++) {
        if (data.entityList[i]->is_wanted_entity(entity_type, entity_id))
            data.entityList.erase(data.entityList.begin() + i);
    }

}

/**
 * @brief checks if the game is pause. Necessary to avoid multiple threads looking at the same variable
 *
 */
bool ClientGame::isPaused() {
    bool value;
    pause_mutex.lock();
    value = Paused;
    pause_mutex.unlock();
    return value;
}

/**
 * @brief modifies the game pause value. Necessary to avoid multiple threads looking at the same variable
 *
 * @param value new pause value
 */
void ClientGame::setPaused(bool value) {
    pause_mutex.lock();
    Paused = value;
    pause_mutex.unlock();
}

/**
 * @brief checks if the game is Stopped. Necessary to avoid multiple threads looking at the same variable
 *
 */
bool ClientGame::isStop() {
    bool value;
    pause_mutex.lock();
    value = Stopping;
    pause_mutex.unlock();
    return value;
}

/**
 * @brief modifies the game stop value. Necessary to avoid multiple threads looking at the same variable
 *
 * @param value new pause value
 */
void ClientGame::setStop(bool value) {
    pause_mutex.lock();
    Stopping = value;
    pause_mutex.unlock();
}

void ClientGame::processNetworkPackets()
{
    auto packets = _netBuffer->popAllPackets();
    for (const auto& pkt : packets) {
        switch (pkt.actionType)
        {
        case 0:
            std::cout << "ENTITY CREATED\n";
            createEntity(pkt.entityType, pkt.entityId, {pkt.posX, pkt.posY});
            break;
        case 1:
            std::cout << "ENTITY MOVED\n";
            moveEntity(pkt.entityType, pkt.entityId, {pkt.posX, pkt.posY});
            break;
        case 2:
            std::cout << "ENTITY DELETED\n";
            deleteEntity(pkt.entityType, pkt.entityId);
            break;
        default:
            break;
        }
    }
}
