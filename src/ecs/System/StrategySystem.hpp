
#pragma once

#include "ISystem.hpp"

class StrategySystem : public ISystem {
    private:
        void executeEntity(IMediatorEntity&, relevant_data_t &) override;
    public:
        StrategySystem();
};

