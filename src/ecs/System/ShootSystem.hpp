
#pragma once

#include "ISystem.hpp"

class ShootSystem : public ISystem {
    private:
        void executeEntity(IMediatorEntity&, relevant_data_t &) override;
    public:
        ShootSystem();
};

