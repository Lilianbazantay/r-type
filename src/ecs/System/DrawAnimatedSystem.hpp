
#pragma once

#include "ISystem.hpp"

class DrawAnimatedSystem : public ISystem {
    private:
        void executeEntity(IMediatorEntity&, relevant_data_t &) override;
    public:
        DrawAnimatedSystem();
};

