#pragma once
#include "../IComponent.hpp"
#include <memory>
#include <vector>

/**
 * @brief Interface for Mediator Entity
 */
class IMediatorEntity
{
    protected:
        // COMPONENT LIST
        std::vector<std::unique_ptr<IComponent>> _undergoerComponents;   // Ex: Healt, Shield, Position ...
        std::vector<std::unique_ptr<IComponent>> _actuatorComponents;    // Ex: Input Catcher, Button, ...
        std::vector<ComponentType> _actuatorTypeList;
        std::vector<ComponentType> _undergoerTypeList;
        // LINKED ENTITIES
        std::vector<IMediatorEntity> _attachedEntities; // Ex: weapon attached to a player, ...

    public:
        ~IMediatorEntity() = default;

        virtual void run() = 0;

        // COMPONENT MANAGER
        std::vector<ComponentType> GetActuatorComponents();
        void AddActuatorComponent(std::unique_ptr<IComponent> component);
        std::vector<ComponentType> GetUnderGoerComponents();
        void AddUndergoerComponent(std::unique_ptr<IComponent> component);

        // CLONE
        virtual IMediatorEntity *Clone() = 0;

        // FIND
        IComponent *FindComponent(ComponentType type);
};
