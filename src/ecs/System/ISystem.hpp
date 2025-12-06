
#pragma once

#include "../Entity/IMediatorEntity.hpp"
#include "ecs/IComponent.hpp"
#include <memory>
#include <vector>

class ISystem {
    private:
        virtual void executeEntity(IMediatorEntity&) = 0;
        bool checkRequirements(IMediatorEntity&);
    protected:
        std::vector<ComponentType> requiedComponents;
        std::vector<ComponentType> optionnalComponents;
    public:
        ~ISystem() = default;

        void addRequiredComponentType(std::shared_ptr<IComponent>);
        void addRequiredComponentType(ComponentType);
        void addRequiredComponentType(std::vector<ComponentType>);

        void addOptionnalComponentType(ComponentType);

        void checkEntity(IMediatorEntity&);
};
