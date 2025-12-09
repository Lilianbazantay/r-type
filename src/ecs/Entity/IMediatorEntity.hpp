#pragma once
#include "../IComponent.hpp"
#include <vector>

/**
 * @brief Interface for Mediator Entity
 */
class IMediatorEntity
{
    private:
        // Components lists
        std::vector<IComponent> _undergoerComponents;   // Ex: Healt, Shield, Position ...
        std::vector<IComponent> _actuatorComponents;    // Ex: Input Catcher, Button, ...
        // Linked Entities
        std::vector<IMediatorEntity> _attachedEntities; // Ex: weapon attached to a player, ...

    public:
        ~IMediatorEntity() = default;

        virtual void run() = 0;

        // Components management
        std::vector<IComponent> GetActuatorComponents();
        void AddActuatorComponent(IComponent component);
        std::vector<IComponent> GetUnderGoerComponents();
        void AddUndergoerComponent(IComponent component);

        IComponent *FindComponent(ComponentType type);
};
