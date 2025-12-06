
#pragma once

#include "ISystem.hpp"

class DrawAnimatedSystem : public ISystem {
    private:
        DrawAnimatedSystem();
        void executeEntity(IMediatorEntity&) override;
    public:
};

