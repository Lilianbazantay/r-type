#include "../client/NetworkBuffer.hpp"
#include "FlappyClientGame.hpp"

int main()
{
    NetworkBuffer net;
    FlappyClientGame game("127.0.0.1", 4242, &net);
    game.Loop();
    return 0;
}
