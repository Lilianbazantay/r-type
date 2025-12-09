
#pragma once

#include "../relevant_data.hpp"
#include "ecs/IComponent.hpp"
#include <memory>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

class ISystem {
    private:
        virtual void executeEntity(IMediatorEntity&, relevant_data_t&) = 0;
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

        void checkEntity(IMediatorEntity&, relevant_data_t&);
};
