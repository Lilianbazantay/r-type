#pragma once

#include "../server/server.hpp"
#include "../server/NetworkServerBuffer.hpp"
#include "../ecs/System/ISystem.hpp"
#include "../ecs/relevant_data.hpp"

#include <memory>
#include <vector>

class NetworkContinuousBuffer;
class NetworkClientBuffer;

class FlappyServerGame {
public:
    FlappyServerGame(int port,
        NetworkServerBuffer* rbuf,
        NetworkClientBuffer* sbuf,
        NetworkContinuousBuffer* cbuf);

    void Loop();

private:
    void Update();
    void spawnPipe();
    void handleFlap(int playerId);

    std::unique_ptr<IMediatorEntity> createPlayer();
    std::unique_ptr<IMediatorEntity> createWall();

    relevant_data_t data;
    std::vector<std::unique_ptr<ISystem>> systems;

    NetworkServerBuffer* recvBuf = nullptr;
    NetworkClientBuffer* sendBuf = nullptr;
    NetworkContinuousBuffer* contBuf = nullptr;

    Server server;

    uint32_t serverTick = 0;
    private:
    float _playerVy = 0.f;
    bool _gameOver = false;
    bool _started = false;
    bool _sentShutdown = false;
    float _lastPipeX = -1000.f;
};
