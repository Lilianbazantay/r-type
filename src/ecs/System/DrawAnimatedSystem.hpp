
#pragma once

#include "ISystem.hpp"

class DrawAnimatedSystem : public ISystem {
    private:
        DrawAnimatedSystem();
        void executeEntity(IMediatorEntity&, relevant_data_t &) override;
    public:
};

