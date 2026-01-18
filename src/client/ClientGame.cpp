#include "ClientGame.hpp"
#include "../ecs/relevant_data.hpp"
#include "../ecs/Component/Position.hpp"
#include "../ecs/Component/Sprite.hpp"
#include "../ecs/Component/Hitbox.hpp"
#include "../ecs/System/DrawAnimatedSystem.hpp"
#include "../ecs/System/DrawSpriteSystem.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <cmath>

static std::pair<float,float> lerp2(const std::pair<float,float>& a,
                                   const std::pair<float,float>& b,
                                   float t)
{
    return { a.first + (b.first - a.first) * t,
             a.second + (b.second - a.second) * t };
}

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

ClientGame::ClientGame(
    std::string ip,
    int port,
    NetworkBuffer *netBuffer,
    EntityFactory& inFactory
)
    : client(ip, port, netBuffer),
      factory(inFactory)
{
    _netBuffer = netBuffer;
    _inputManager.setClientGame(this);
    data.window.create(sf::VideoMode({1920, 1080}), "RTYPE");
    data.window.clear(sf::Color::Black);
    data.window.setActive(true);

    data.bullet_count = 0;
    data.enemy_count = 0;

    clock.restart();
    Prevtime = clock.getElapsedTime();

    _inputManager.setClient(&client);

    systemList.push_back(std::make_unique<DrawSpriteSystem>());
    systemList.push_back(std::make_unique<DrawAnimatedSystem>());

    factory.registerComponentConstructors();
    factory.registerPrototypes({
        {"Background", {"Background", loadJsonFromFile("./configuration/background/Background.json")}},
        {"Player", {"Player", loadJsonFromFile("./configuration/player/Player.json")}},
        {"Enemy", {"Enemy", loadJsonFromFile("./configuration/enemy/Enemy.json")}},
        {"PlayerBullet", {"PlayerBullet", loadJsonFromFile("./configuration/weapon/bullet/PlayerBullet.json")}},
        {"EnemyBullet", {"EnemyBullet", loadJsonFromFile("./configuration/weapon/bullet/EnemyBullet.json")}}
    });
    createEntity(ENTITY_BACKGROUND, 0, {0.f, 0.f});

    data.window.setKeyRepeatEnabled(false);

    std::string IP = client.getServerIP();
    uint32_t ipValue = 0;
    unsigned a, b, c, d;

    if (sscanf(IP.c_str(), "%u.%u.%u.%u", &a, &b, &c, &d) == 4)
        ipValue = (a << 24u) | (b << 16u) | (c << 8u) | d;
    else
        std::cout << "Invalid IP format\n";

    client.sendConnectionRequest(ipValue, client.getServerPort());
    client.sendStartGame();
    client.start();
}

bool ClientGame::IsEntityExist(int type, int id)
{
    for (const auto& e : data.entityList)
        if (e->getType() == type && e->getId() == id)
            return true;
    return false;
}

void ClientGame::Update()
{
    sf::Time Newtime = clock.getElapsedTime();
    data.runtime =
        (Newtime.asMicroseconds() - Prevtime.asMicroseconds()) / 1'000'000.f;
    Prevtime = Newtime;

    for (auto& system : systemList)
        for (auto& entity : data.entityList)
            system->checkEntity(*entity, data);
}

/**
 * @brief main loop for ClientGame. clear window, display, and retrieves events.
 *
 */
void ClientGame::Loop() {
    while(data.window.isOpen()) {
        sf::Event evt;

        if (Stopping)
            return;

        data.window.clear(sf::Color::Black);

        if (!Paused) {
            processNetworkPackets();
            applyNetworkInterpolation();
            Update();
        }

        data.window.display();

        while (data.window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed)
                data.window.close();
            if (evt.type == sf::Event::Resized)
                handleResize({evt.size.width, evt.size.height});
            _inputManager.processEvent(evt);
        }
    }
}

void ClientGame::handleResize(std::pair<unsigned int, unsigned int> _newSize)
{
    prevWindowSize = actWindowSize;
    actWindowSize = _newSize;

    for (auto& entity : data.entityList) {
        auto* sprite   = dynamic_cast<Sprite*>(entity->FindComponent(ComponentType::SPRITE));
        auto* hitbox   = dynamic_cast<Hitbox*>(entity->FindComponent(ComponentType::HITBOX));

        if (sprite) {
            auto size = sprite->GetSize();
            sprite->SetSize(
                size.first  / actWindowSize.first  * prevWindowSize.first,
                size.second / actWindowSize.second * prevWindowSize.second
            );
        }

        if (hitbox) {
            auto size = hitbox->GetHitboxSize();
            hitbox->SetHitboxSize(
                size.first  / actWindowSize.first  * prevWindowSize.first,
                size.second / actWindowSize.second * prevWindowSize.second
            );
        }
    }
}

void ClientGame::createEntity(
    int entity_type,
    int entity_id,
    std::pair<float, float> position
)
{
    if (IsEntityExist(entity_type, entity_id))
        return;

    std::unique_ptr<IMediatorEntity> entity;

/**
 * @brief creates an entity based on its entity type, and assigns it an ID and a position
 */
void ClientGame::createEntity(int entity_type, int entity_id, std::pair<float, float> position) {
    int prevSize = data.entityList.size();
    switch (entity_type) {
        case ENTITY_BACKGROUND:
            entity = std::make_unique<Background>(factory);
            break;
        case ENTITY_PLAYER:
            entity = std::make_unique<Player>(factory);
            break;
        case ENTITY_ENEMY:
            entity = std::make_unique<Enemy>(factory);
            break;
        case ENTITY_BULLET:
            entity = std::make_unique<PlayerBullet>(factory);
            break;
        default:
            return;
    }

    entity->setId(entity_id);
    data.entityList.push_back(std::move(entity));

    auto* pos = dynamic_cast<Position*>(
        data.entityList.back()->FindComponent(ComponentType::POSITION)
    );
    if (pos)
        pos->SetPosition(position);
}

/**
 * @brief changes the entity position,
 */
void ClientGame::moveEntity(int entity_type, int entity_id, std::pair<float, float> new_position) {
    for (size_t i = 0; i < data.entityList.size(); i++) {
        if (!data.entityList[i]->is_wanted_entity(entity_type, entity_id))
            continue;

        auto* pos = dynamic_cast<Position*>(e->FindComponent(ComponentType::POSITION));
        if (pos)
            pos->SetPosition(new_position);
        return;
    }
}

/**
 * @brief deletes the specified entity
 */
void ClientGame::deleteEntity(int entity_type, int entity_id) {
    std::cout << "[DELETE] looking for type=" << entity_type
              << " id=" << entity_id << "\n";

    for (const auto& e : data.entityList) {
        std::cout << "  entity type=" << e->getType()
                  << " id=" << e->getId() << "\n";
    }
    std::erase_if(data.entityList, [&](const auto& e) {
        return e->getType() == entity_type && e->getId() == entity_id;
    });
}

/**
 * @brief checks if the game is pause. Necessary to avoid multiple threads looking at the same variable
 */
bool ClientGame::isPaused() {
    bool value;
    pause_mutex.lock();
    value = Paused;
    pause_mutex.unlock();
    return value;
}

void ClientGame::setPaused(bool value) {
    pause_mutex.lock();
    Paused = value;
}

bool ClientGame::isStop() {
    bool value;
    pause_mutex.lock();
    value = Stopping;
    pause_mutex.unlock();
    return value;
}

void ClientGame::setStop(bool value) {
    pause_mutex.lock();
    Stopping = value;
}

void ClientGame::pushSnapshot(uint8_t type, uint16_t id, uint32_t tick, float x, float y)
{
    uint32_t key = makeKey(type, id);
    _latestServerTick = std::max(_latestServerTick, tick);

    auto &q = _netBuffers[key].snaps;
    if (!q.empty() && q.back().tick == tick) {
        q.back().pos = {x, y};
    } else {
        q.push_back({tick, {x, y}});
    }

    while (q.size() > MAX_SNAPSHOTS)
        q.pop_front();
}

void ClientGame::applyNetworkInterpolation()
{
    if (_latestServerTick <= INTERP_DELAY_TICKS)
        return;

    uint32_t renderTick = _latestServerTick - INTERP_DELAY_TICKS;

    std::vector<uint32_t> toErase;

    for (auto &[key, buf] : _netBuffers) {
        auto &snaps = buf.snaps;
        if (snaps.size() < 2)
            continue;
        int type = int((key >> 16) & 0xFF);
        int id   = int(key & 0xFFFF);
        IMediatorEntity *ent = nullptr;
        for (auto &ptr : data.entityList) {
            if (ptr->is_wanted_entity(type, id)) {
                ent = ptr.get();
                break;
            }
        }
        if (!ent) {
            toErase.push_back(key);
            continue;
        }
        Snapshot *a = nullptr;
        Snapshot *b = nullptr;
        for (size_t i = 0; i + 1 < snaps.size(); ++i) {
            if (snaps[i].tick <= renderTick && renderTick <= snaps[i + 1].tick) {
                a = &snaps[i];
                b = &snaps[i + 1];
                break;
            }
        }
        std::pair<float,float> finalPos = snaps.back().pos;
        if (a && b && b->tick != a->tick) {
            float t = float(renderTick - a->tick) / float(b->tick - a->tick);
            finalPos = lerp2(a->pos, b->pos, t);
        } else {
            const auto &s0 = snaps[snaps.size() - 2];
            const auto &s1 = snaps[snaps.size() - 1];

            uint32_t dtTicks = 0;
            if (renderTick > s1.tick)
                dtTicks = std::min<uint32_t>(renderTick - s1.tick, MAX_EXTRAP_TICKS);

            float denom = float(std::max<uint32_t>(1, s1.tick - s0.tick));
            float vx = (s1.pos.first  - s0.pos.first)  / denom;
            float vy = (s1.pos.second - s0.pos.second) / denom;

            finalPos = { s1.pos.first  + vx * dtTicks,
                         s1.pos.second + vy * dtTicks };
        }
        Position *pos = dynamic_cast<Position*>(ent->FindComponent(ComponentType::POSITION));
        if (pos) {
            pos->SetPosition(finalPos);
        }
        while (snaps.size() >= 2 && snaps[1].tick < renderTick) {
            snaps.pop_front();
        }
    }
    for (auto key : toErase) {
        _netBuffers.erase(key);
    }
}

void ClientGame::processNetworkPackets()
{
    auto packets = _netBuffer->popAllPackets();

    for (size_t i = 0; i < packets.size(); i++) {
        const auto &p = packets[i];
        switch ((int)p.actionType)
        {
        case 0: {
            createEntity((int)p.entityType, (int)p.entityId, {(float)p.posX, (float)p.posY});
            pushSnapshot(p.entityType, p.entityId, p.serverTick, (float)p.posX, (float)p.posY);
            break;
        }
        case 1: {
            pushSnapshot(p.entityType, p.entityId, p.serverTick, (float)p.posX, (float)p.posY);
            break;
        }
        case 2: {
            deleteEntity((int)p.entityType, (int)p.entityId);
            uint32_t key = makeKey(p.entityType, p.entityId);
            _netBuffers.erase(key);

            break;
        }
        case 14:
            setStop(true);
            break;
        default:
            break;
        }
    }
}

void ClientGame::predictLocalMove(float dx, float dy)
{
    if (_localPlayerId < 0)
        return;

    for (auto &e : data.entityList) {
        if (!e->is_wanted_entity(ENTITY_PLAYER, _localPlayerId))
            continue;
        Position *p = dynamic_cast<Position*>(e->FindComponent(ComponentType::POSITION));
        if (!p)
            return;
        auto pos = p->GetPosition();
        // ptet ajuster ca
        float speed = 300.f;
        pos.first  += dx * speed * data.runtime;
        pos.second += dy * speed * data.runtime;
        p->SetPosition(pos);
        return;
    }
}
