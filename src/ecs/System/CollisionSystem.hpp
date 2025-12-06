
#pragma once

#include "ISystem.hpp"

class CollisionSystem : public ISystem {
    private:
        CollisionSystem();
        void executeEntity(IMediatorEntity&) override;
    public:
};
