
#pragma once

#include "../relevant_data.hpp"
#include "../IComponent.hpp"
#include <memory>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

class ISystem {
    private:
        bool checkRequirements(IMediatorEntity&);
    protected:
        virtual void executeEntity(IMediatorEntity&, relevant_data_t&) = 0;
        std::vector<ComponentType> requiedComponents;
        std::vector<ComponentType> optionnalComponents;
    public:
        virtual ~ISystem() = default;

        void addRequiredComponentType(std::shared_ptr<IComponent>);
        void addRequiredComponentType(ComponentType);
        void addRequiredComponentType(std::vector<ComponentType>);

        void addOptionnalComponentType(ComponentType);

        void checkEntity(IMediatorEntity&, relevant_data_t&);
};
