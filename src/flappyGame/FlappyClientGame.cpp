#include "FlappyClientGame.hpp"

#include "../ecs/Component/Position.hpp"
#include "../ecs/Entity/Entities.hpp"
#include "ecs/Component/AnimatedSprite.hpp"
#include "ecs/Component/Direction.hpp"
#include "ecs/Component/Gravity.hpp"
#include "ecs/Component/Hitbox.hpp"
#include "ecs/Component/Sprite.hpp"
#include "ecs/Component/Velocity.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/IComponent.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <memory>

namespace {
    inline float clampf(float v, float lo, float hi) {
        return std::clamp(v, lo, hi);
    }

    static constexpr float WIN_W = 1280.f;
    static constexpr float WIN_H = 720.f;

    static constexpr float PIPE_W = 90.f;
    static constexpr float GAP_H  = 200.f;
}

std::unique_ptr<IMediatorEntity> FlappyClientGame::createPlayer() {
    auto _blankEntity = std::make_unique<BlankEntity>();

    _blankEntity->setType(ENTITY_PLAYER);
    _blankEntity->AddActuatorComponent(std::make_unique<Position>(100, 500));
    _blankEntity->AddActuatorComponent(std::make_unique<Gravity>(true, 100));
    _blankEntity->AddActuatorComponent(std::make_unique<Velocity>(0));
    _blankEntity->AddActuatorComponent(std::make_unique<Direction>(0, 0));
    _blankEntity->AddActuatorComponent(std::make_unique<AnimatedSprite>("./assets/Bird1-1.png",
        std::make_pair(3.f, 3.f),
        std::make_pair(16.f, 16.f)));
    _blankEntity->AddActuatorComponent(std::make_unique<Hitbox>((size_t)16, (size_t)16, 0,
        std::vector<int>{1}, std::vector<int>{1}));
    AnimatedSprite* _animation = dynamic_cast<AnimatedSprite*>(_blankEntity->FindComponent(ComponentType::ANIMATED_SPRITE));
    if (_animation != nullptr)
        _animation->addAnimation({0, 0}, {16, 16}, {16, 0}, IDLE, true, 3);
    return _blankEntity;
}

std::unique_ptr<IMediatorEntity> FlappyClientGame::createWall() {
    auto _blankEntity = std::make_unique<BlankEntity>();

    _blankEntity->setType(ENTITY_ENEMY);
    _blankEntity->AddActuatorComponent(std::make_unique<Position>(1920, 1070));
    _blankEntity->AddActuatorComponent(std::make_unique<Sprite>("./assets/Enemy.png", 32, 32));
    return _blankEntity;
}

std::unique_ptr<IMediatorEntity> FlappyClientGame::createBackGround() {
    auto _blankEntity = std::make_unique<BlankEntity>();

    _blankEntity->setType(ENTITY_BACKGROUND);
    _blankEntity->AddActuatorComponent(std::make_unique<Position>(1920, 1070));
    _blankEntity->AddActuatorComponent(std::make_unique<Sprite>("./assets/Background5.png", 1920, 1080));
    return _blankEntity;
}


FlappyClientGame::FlappyClientGame(std::string ip, int port, NetworkBuffer* net)
    : client(ip, port, net), netBuf(net)
{
    data.window.create({(unsigned)WIN_W, (unsigned)WIN_H}, "Flappy ECS Net");
    data.window.setActive(true);
    data.window.setKeyRepeatEnabled(false);
    const std::string birdPath  = std::string(PROJECT_ROOT_PATH) + "assets/Bird1-1.png";
    const std::string BackgroundPath = std::string(PROJECT_ROOT_PATH) + "assets/Background5.png";

    if (_texBird.loadFromFile(birdPath)) {
        _sprBird.setTexture(_texBird);
        const sf::Vector2u texSize = _texBird.getSize();
        constexpr int FRAME_COUNT = 4;
        int frameWidth  = static_cast<int>(texSize.x / FRAME_COUNT);
        int frameHeight = static_cast<int>(texSize.y);
        _sprBird.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        _sprBird.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
        _sprBird.setScale(2.f, 2.f);
    } else {
        std::cerr << "[FlappyClient] WARN: cannot load " << birdPath << " (fallback rect)\n";
    }

    if (_texBackground.loadFromFile(BackgroundPath)) {
        _sprBackground.setTexture(_texBackground);
        _sprBackground.setScale(6.f, 3.f);
    } else {
        std::cerr << "[FlappyClient] WARN: cannot load " << BackgroundPath << " (no background)\n";
    }

    client.start();

    std::string IP = client.getServerIP();
    uint32_t ipValue = 0;
    unsigned oa, ob, oc, od;
    if (sscanf(IP.c_str(), "%u.%u.%u.%u", &oa, &ob, &oc, &od) == 4)
        ipValue = (oa << 24u) | (ob << 16u) | (oc << 8u) | od;

    client.sendConnectionRequest(ipValue, client.getServerPort());
    client.sendStartGame();
}

std::unique_ptr<IMediatorEntity> FlappyClientGame::makeEntityFromType(int entityType)
{
    switch (entityType) {
        case ENTITY_PLAYER:     return createPlayer();
        case ENTITY_ENEMY:      return createWall();
        case ENTITY_BACKGROUND: return createBackGround();
        default:                return nullptr;
    }
}

void FlappyClientGame::processPackets()
{
    auto packets = netBuf->popAllPackets();

    for (auto& p : packets) {
        const int action = (int)p.actionType;
        const int type   = (int)p.entityType;
        const int id     = (int)p.entityId;

        float fx = clampf((float)p.posX, 0.f, WIN_W);
        float fy = clampf((float)p.posY, 0.f, WIN_H);

        if (action == 0) {
            bool exists = false;
            for (auto &e : data.entityList) {
                if (e->is_wanted_entity(type, id)) { exists = true; break; }
            }
            if (exists)
                continue;

            auto ent = makeEntityFromType(type);
            if (!ent)
                continue;

            ent->setId(id);

            auto* pos = dynamic_cast<Position*>(ent->FindComponent(ComponentType::POSITION));
            if (pos)
                pos->SetPosition({fx, fy});

            data.entityList.push_back(std::move(ent));
            continue;
        }

        if (action == 1) {
            for (auto& e : data.entityList) {
                if (!e->is_wanted_entity(type, id))
                    continue;

                auto* pos = dynamic_cast<Position*>(e->FindComponent(ComponentType::POSITION));
                if (pos)
                    pos->SetPosition({fx, fy});
                break;
            }
            continue;
        }

        if (action == 2) {
            std::erase_if(data.entityList, [&](const auto& ent) {
                return ent->is_wanted_entity(type, id);
            });
            continue;
        }
        if ((int)p.actionType == 14) {
            data.window.close();
            return;
        }
    }
}

void FlappyClientGame::Loop()
{
    while (data.window.isOpen()) {
        sf::Event ev;
        while (data.window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                data.window.close();
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
                data.window.close();
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Space)
                client.sendInput(true, 0);
            if (ev.type == sf::Event::KeyReleased && ev.key.code == sf::Keyboard::Space)
                client.sendInput(false, 0);
        }

        processPackets();

        data.window.clear(sf::Color::Black);

        if (_texBackground.getSize().x > 0) {
            _sprBackground.setPosition(0.f, 0.f);
            data.window.draw(_sprBackground);
        }

        for (auto &e : data.entityList) {
            if (e->getType() != ENTITY_ENEMY)
                continue;
            auto *pos = dynamic_cast<Position*>(e->FindComponent(ComponentType::POSITION));
            if (!pos)
                continue;
            auto pp = pos->GetPosition();
            float x = pp.first;
            float gapY = pp.second;
            float gapTop = gapY - GAP_H * 0.5f;
            float gapBot = gapY + GAP_H * 0.5f;
            if (gapTop > 0.f) {
                sf::RectangleShape top(sf::Vector2f(PIPE_W, gapTop));
                top.setPosition(x - PIPE_W * 0.5f, 0.f);
                top.setFillColor(sf::Color(0, 200, 0));
                data.window.draw(top);
            }
            if (gapBot < WIN_H) {
                sf::RectangleShape bot(sf::Vector2f(PIPE_W, WIN_H - gapBot));
                bot.setPosition(x - PIPE_W * 0.5f, gapBot);
                bot.setFillColor(sf::Color(0, 200, 0));
                data.window.draw(bot);
            }
        }

        for (auto &e : data.entityList) {
            if (e->getType() != ENTITY_PLAYER)
                continue;
            auto *pos = dynamic_cast<Position*>(e->FindComponent(ComponentType::POSITION));
            if (!pos)
                break;
            auto p = pos->GetPosition();
            if (_texBird.getSize().x > 0) {
                _sprBird.setPosition(p.first, p.second);
                data.window.draw(_sprBird);
            } else {
                sf::RectangleShape r(sf::Vector2f(30.f, 20.f));
                r.setOrigin(15.f, 10.f);
                r.setPosition(p.first, p.second);
                r.setFillColor(sf::Color::Yellow);
                data.window.draw(r);
            }
            break;
        }

        data.window.display();
    }
}
