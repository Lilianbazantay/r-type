
#pragma once

#include "ISystem.hpp"

class MovementSystem : public ISystem {
    private:
        void executeEntity(IMediatorEntity&, relevant_data_t &) override;
    public:
        MovementSystem();
};
