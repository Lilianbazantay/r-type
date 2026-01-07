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
#include <string>

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
    std::string IP = client.getServerIP();
    uint32_t ipValue = 0;
    unsigned a, b, c, d;
    if (sscanf(IP.c_str(), "%u.%u.%u.%u", &a, &b, &c, &d) == 4)
        ipValue = (a << 24u) | (b << 16u) | (c << 8u) | d;
    else
        std::cout << "Invalid IP format\n";
    client.sendConnectionRequest(ipValue, client.getServerPort());
    client.sendStartGame();
}

bool ClientGame::IsEntityExist(int type, int id) {
    std::vector<std::unique_ptr<IMediatorEntity>> &list = data.entityList;
    for (size_t i = 0; i < list.size(); i++)
        if (list[i]->getType() == type && list[i]->getId() == id)
            return true;
    return false;
}


/**
 * @brief updates the deltatime(runtime), and go through every entity and system.
 *
 */
void ClientGame::Update() {
    sf::Time Newtime = clock.getElapsedTime();
    data.runtime = (Newtime.asMicroseconds() - Prevtime.asMicroseconds()) / 1000000.;
    Prevtime = Newtime;
    for (size_t i = 0; i < systemList.size(); i++)
        for (size_t j = 0; j < data.entityList.size(); j++)
            systemList[i]->checkEntity(*data.entityList[j], data);
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
            if (!IsEntityExist(entity_type, entity_id))
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
    std::cout << "[DELETE] looking for type=" << entity_type
              << " id=" << entity_id << "\n";

    for (const auto& e : data.entityList) {
        std::cout << "  entity type=" << e->getType()
                  << " id=" << e->getId() << "\n";
    }

    std::erase_if(data.entityList, [&](const auto& e) {
        return e->is_wanted_entity(entity_type, entity_id);
    });
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
//    if (packets.size() != 0)
//        std::cout << "Packet Size: " << packets.size() << ", " << (int)packets[0].actionType << std::endl;
    for (size_t i = 0; i < packets.size(); i++) {
        switch ((int)packets[i].actionType)
        {
        case 0:
            //std::cout << "ENTITY CREATED\n";
            createEntity((int)packets[i].entityType, (int)packets[i].entityId, {(int)packets[i].posX, (int)packets[i].posY});
            break;
        case 1:
            //std::cout << "ENTITY MOVED\n";
            moveEntity((int)packets[i].entityType, (int)packets[i].entityId, {(int)packets[i].posX, (int)packets[i].posY});
            break;
        case 2:
            //std::cout << "ENTITY DELETED\n";
            std::cout << "Message actionType: " << (int)packets[i].actionType << "\n";
            std::cout << "Message affected entity: " << (int)packets[i].entityId << " type: " << (int)packets[i].entityType << "\n";
            deleteEntity((int)packets[i].entityType, (int)packets[i].entityId);
            break;
        case 14:
            setStop(true);
        default:
            break;
        }
    }
}
