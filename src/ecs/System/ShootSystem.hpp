
#pragma once

#include "ISystem.hpp"

class ShootSystem : public ISystem {
    private:
        ShootSystem();
        void executeEntity(IMediatorEntity&, relevant_data_t &) override;
    public:
};

