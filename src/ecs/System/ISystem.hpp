
#pragma once

#include "../Entity/IMediatorEntity.hpp"
#include "ecs/IComponent.hpp"
#include <memory>
#include <vector>

class ISystem {
    private:
        std::vector<ComponentType> requiedComponents;

        virtual void executeEntity(IMediatorEntity&) = 0;
        bool checkRequirements(IMediatorEntity&);
    public:
        ~ISystem() = default;

        void addRequiredComponent(std::shared_ptr<IComponent> component);
        void checkEntity(IMediatorEntity&);
};
