
#pragma once

#include "ISystem.hpp"

class MovementSystem : public ISystem {
    private:
        MovementSystem();
        void executeEntity(IMediatorEntity&, relevant_data_t &) override;
    public:
};
