
#pragma once

#include "ISystem.hpp"

class DrawSpriteSystem : public ISystem {
    private:
        void executeEntity(IMediatorEntity&, relevant_data_t &) override;
    public:
        DrawSpriteSystem();
};

