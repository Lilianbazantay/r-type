#include "../server/ServerGame.hpp"
#include "FlappyServerGame.hpp"

int main()
{
    NetworkServerBuffer rbuf;
    NetworkClientBuffer sbuf;
    NetworkContinuousBuffer cbuf;
    FlappyServerGame game(4242, &rbuf, &sbuf, &cbuf);
    game.Loop();
    return 0;
}
