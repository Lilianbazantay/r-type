
#pragma once

#include "ISystem.hpp"

class DrawSpriteSystem : public ISystem {
    private:
        DrawSpriteSystem();
        void executeEntity(IMediatorEntity&) override;
    public:
};

