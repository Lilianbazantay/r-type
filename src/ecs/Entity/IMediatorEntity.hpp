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
        std::vector<IComponent> undergoerComponents;   // Ex: Healt, Shield, Position ...
        std::vector<IComponent> actuatorComponents;    // Ex: Input Catcher, Button, ...
        // Linked Entities
        std::vector<IMediatorEntity> attachedEntities; // Ex: weapon attached to a player, ...

    public:
        ~IMediatorEntity() = default;

        virtual void run() = 0;
};
