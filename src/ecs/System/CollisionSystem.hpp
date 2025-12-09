
#pragma once

#include "ISystem.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include <vector>

class CollisionSystem : public ISystem {
    private:
        void executeEntity(IMediatorEntity&, relevant_data_t &) override;
        bool checkCollison(std::pair<float, float>, std::pair<float, float>, std::pair<float, float>, std::pair<float, float>);
        bool checkLayers(std::vector<int>, std::vector<int>);
    public:
        CollisionSystem();
};
