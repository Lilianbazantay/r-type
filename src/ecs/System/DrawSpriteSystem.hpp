
#pragma once

#include "ISystem.hpp"

class DrawSpriteSystem : public ISystem {
    private:
        DrawSpriteSystem();
        void executeEntity(IMediatorEntity&, relevant_data &) override;
    public:
};

