#include "FlappyServerGame.hpp"

#include "ecs/Component/Position.hpp"
#include "ecs/Entity/Entities.hpp"
#include "server/encoder.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>

static constexpr float PIPE_SPEED     = 240.f;
static constexpr float PIPE_SPAWN_X   = 1200.f;
static constexpr float PIPE_DESPAWN_X = -200.f;
static constexpr float PIPE_MIN_DISTANCE = 200.f;

static constexpr float GRAVITY = 1200.f;
static constexpr float FLAP_VY = -520.f;

static constexpr float FLOOR_Y = 650.f;
static constexpr float CEIL_Y  = 50.f;

static constexpr float BIRD_X  = 200.f;

static constexpr float PIPE_W  = 90.f;
static constexpr float GAP_H   = 200.f;
static constexpr float BIRD_R  = 18.f;

static constexpr float PIPE_SPAWN_PERIOD = 1.35f;

static float computeDeltaSeconds()
{
    using clock = std::chrono::steady_clock;
    static auto last = clock::now();
    auto now = clock::now();
    std::chrono::duration<float> dt = now - last;
    last = now;
    float d = dt.count();
    if (d < 0.f) d = 0.f;
    if (d > 0.05f) d = 0.05f;
    return d;
}

static inline uint16_t nextPacketId(Server &srv)
{
    return static_cast<uint16_t>(srv.currentID++);
}

FlappyServerGame::FlappyServerGame(int port,
    NetworkServerBuffer* r,
    NetworkClientBuffer* s,
    NetworkContinuousBuffer* c)
    :   recvBuf(r), sendBuf(s), contBuf(c),
        server(port, r, s, c),
        _playerVy(0.f),
        _gameOver(false),
        _started(false),
        _sentShutdown(false),
        _lastPipeX(-1000.f)
{
    std::srand((unsigned)std::time(nullptr));
    server.start();

    auto player = std::make_unique<Player>();
    player->setId(1);

    auto *pos = dynamic_cast<Position*>(player->FindComponent(ComponentType::POSITION));
    if (pos) pos->SetPosition({BIRD_X, 300.f});

    data.entityList.push_back(std::move(player));
}

void FlappyServerGame::handleFlap(int playerId)
{
    (void)playerId;
    _playerVy = FLAP_VY;
}

void FlappyServerGame::spawnPipe()
{
    static int pipeId = 100;

    auto pipe = std::make_unique<Enemy>();
    pipe->setId(pipeId++);

    auto* pos = dynamic_cast<Position*>(pipe->FindComponent(ComponentType::POSITION));
    if (!pos) return;

    if (PIPE_SPAWN_X - _lastPipeX < PIPE_MIN_DISTANCE)
        return;

    float gapY;
    static int initial = 0;
    if (initial < 3) {
        gapY = 360.f;
        initial++;
    } else {
        gapY = float((std::rand() % 380) + 160);
}

pos->SetPosition({PIPE_SPAWN_X, gapY});
_lastPipeX = PIPE_SPAWN_X;


    int id = pipe->getId();
    data.entityList.push_back(std::move(pipe));

    if (_started) {
        auto *ppos = dynamic_cast<Position*>(data.entityList.back()->FindComponent(ComponentType::POSITION));
        if (ppos) {
            auto p = ppos->GetPosition();
            auto pkt = PacketEncoder::encodeCreate(
                nextPacketId(server),
                (uint8_t)ENTITY_ENEMY,
                (uint16_t)id,
                (uint16_t)p.first,
                (uint16_t)p.second
            );
            sendBuf->pushPacket(pkt);
        }
    }
}

void FlappyServerGame::Update()
{
    data.runtime = computeDeltaSeconds();
    serverTick++;
    auto packets = recvBuf->popAllPackets();

    for (auto& p : packets) {
        if (p.getActionType() == ActionType::START_GAME ||
            p.getActionType() == ActionType::NEW_CONNECTION ||
            p.getActionType() == ActionType::PLAYER_CONNECT) {
            _started = true;
        }
        if (p.getActionType() == ActionType::INPUT_PRESSED) {
            _started = true;
            handleFlap(p.getPlayerId());
        }
    }

    if (!_started)
        return;
    static bool sentInitialCreates = false;

    if (!sentInitialCreates) {
        for (auto &e : data.entityList) {
            if (e->getType() == ENTITY_PLAYER) {
                auto *ppos = dynamic_cast<Position*>(e->FindComponent(ComponentType::POSITION));
                if (!ppos) break;
                auto p = ppos->GetPosition();
                auto pkt = PacketEncoder::encodeCreate(
                    nextPacketId(server),
                    (uint8_t)ENTITY_PLAYER,
                    (uint16_t)e->getId(),
                    (uint16_t)p.first,
                    (uint16_t)p.second
                );
                sendBuf->pushPacket(pkt);
                break;
            }
        }
        for (int i = 0; i < 3; ++i)
            spawnPipe();
        sentInitialCreates = true;
        }

    if (_gameOver) {
        if (!_sentShutdown) {
            auto shut = PacketEncoder::encodeSHUTDOWN(nextPacketId(server));
            sendBuf->pushPacket(shut);
            _sentShutdown = true;
        }
        return;
    }
    IMediatorEntity *bird = nullptr;

    for (auto &e : data.entityList) {
        if (e->getType() == ENTITY_PLAYER) {
            bird = e.get();
            break;
        }
    }

    if (!bird)
        return;
    auto *bpos = dynamic_cast<Position*>(bird->FindComponent(ComponentType::POSITION));
    if (!bpos)
        return;

    _playerVy += GRAVITY * (float)data.runtime;
    auto bp = bpos->GetPosition();
    bp.first = BIRD_X;
    bp.second += _playerVy * (float)data.runtime;

    if (bp.second > FLOOR_Y || bp.second < CEIL_Y) {
        _gameOver = true;
    } else {
        bpos->SetPosition(bp);
    }
    static float pipeAcc = 0.f;
    pipeAcc += (float)data.runtime;

    if (pipeAcc >= PIPE_SPAWN_PERIOD) {
        pipeAcc = 0.f;
        spawnPipe();
    }

    if (!_gameOver) {
        float bx = bp.first;
        float by = bp.second;

        for (auto &e : data.entityList) {
            if (e->getType() != ENTITY_ENEMY)
                continue;
            auto *ppos = dynamic_cast<Position*>(e->FindComponent(ComponentType::POSITION));
            if (!ppos) continue;
            auto pp = ppos->GetPosition();
            pp.first -= PIPE_SPEED * (float)data.runtime;
            _lastPipeX = std::max(_lastPipeX, pp.first);
            if (pp.first < PIPE_DESPAWN_X) {
                pp.first = PIPE_SPAWN_X;
                pp.second = float((std::rand() % 380) + 160);
            }
            ppos->SetPosition(pp);
            bool inX = (bx + BIRD_R > pp.first - PIPE_W * 0.5f) && (bx - BIRD_R < pp.first + PIPE_W * 0.5f);
            if (inX) {
                float gapTop = pp.second - GAP_H * 0.5f;
                float gapBot = pp.second + GAP_H * 0.5f;
                bool hitTop = (by - BIRD_R) < gapTop;
                bool hitBot = (by + BIRD_R) > gapBot;
                if (hitTop || hitBot) {
                    _gameOver = true;
                    break;
                }
            }
        }
    }

    if (_gameOver) {
        if (!_sentShutdown) {
            auto shut = PacketEncoder::encodeSHUTDOWN(nextPacketId(server));
            sendBuf->pushPacket(shut);
            _sentShutdown = true;
        }
        return;
    }

    for (auto& e : data.entityList) {
        auto* pos = dynamic_cast<Position*>(e->FindComponent(ComponentType::POSITION));
        if (!pos) continue;
        auto p = pos->GetPosition();
        auto pkt = PacketEncoder::encodeMove(
            nextPacketId(server),
            (uint8_t)e->getType(),
            (uint16_t)e->getId(),
            (uint16_t)std::clamp(p.first,  0.f, 4095.f),
            (uint16_t)std::clamp(p.second, 0.f, 4095.f)
        );

        sendBuf->pushPacket(pkt);
    }
}

void FlappyServerGame::Loop()
{
    while (true) {
        Update();
        usleep(16000);
    }
}
