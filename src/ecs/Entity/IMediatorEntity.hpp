#pragma once
#include "../IComponent.hpp"
#include <memory>
#include <mutex>
#include <vector>

constexpr int ENTITY_BACKGROUND = -1;
constexpr int ENTITY_PLAYER = 0;
constexpr int ENTITY_ENEMY = 1;
constexpr int ENTITY_BULLET = 2;


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

        int id;
        int Entitytype;
        std::mutex _mutex;
    public:
        virtual ~IMediatorEntity() = 0;
        virtual void run() = 0;

        // COMPONENT MANAGER
        std::vector<ComponentType> GetActuatorComponents();
        void AddActuatorComponent(std::unique_ptr<IComponent> component);
        std::vector<ComponentType> GetUnderGoerComponents();
        void AddUndergoerComponent(std::unique_ptr<IComponent> component);

        void setId(int);
        int getId();
        void setType(int);
        int getType();
        bool is_wanted_entity(int, int);

        void lock();
        void unlock();
        // CLONE
        virtual IMediatorEntity *Clone() = 0;

        // FIND
        IComponent *FindComponent(ComponentType type);
};
