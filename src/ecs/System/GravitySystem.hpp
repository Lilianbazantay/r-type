
#pragma once

#include "ISystem.hpp"

class GravitySystem : public ISystem {
    private:
        GravitySystem();
        void executeEntity(IMediatorEntity&) override;
    public:
};

