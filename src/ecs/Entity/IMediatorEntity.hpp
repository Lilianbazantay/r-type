#pragma once

#include "../IComponent.hpp"
#include <memory>
#include <mutex>
#include <vector>

constexpr int ENTITY_BACKGROUND = 0;
constexpr int ENTITY_PLAYER = 1;
constexpr int ENTITY_ENEMY = 2;
constexpr int ENTITY_BULLET = 3;
constexpr int ENTITY_UNDEFINED = 4;

class EntityFactory;

/**
 * @brief Interface for Mediator Entity
 */
class IMediatorEntity
{
protected:
    // COMPONENT LIST
    std::vector<std::unique_ptr<IComponent>> _undergoerComponents;
    std::vector<std::unique_ptr<IComponent>> _actuatorComponents;
    std::vector<ComponentType> _actuatorTypeList;
    std::vector<ComponentType> _undergoerTypeList;

    // LINKED ENTITIES
    std::vector<std::unique_ptr<IMediatorEntity>> _attachedEntities;

    int id = -1;
    int Entitytype = ENTITY_UNDEFINED;
    std::mutex _mutex;
    bool entity_created = true;
    bool entity_changed = false;
    bool entity_live = true;

public:
    virtual ~IMediatorEntity() = default;
    virtual void run() = 0;

    // COMPONENT MANAGER
    std::vector<ComponentType> GetActuatorComponents() const;
    void AddActuatorComponent(std::unique_ptr<IComponent> component);

    std::vector<ComponentType> GetUnderGoerComponents() const;
    void AddUndergoerComponent(std::unique_ptr<IComponent> component);

    // ENTITY META
    void setId(int);
    int getId() const;

    void setType(int);
    int getType() const;

    bool is_wanted_entity(int type, int id) const;

    // STATE
    void lock();
    void unlock();

    bool justCreated();
    bool hasChanged();
    void hasChanged(bool);

    void Alive(bool);
    bool is_Alive() const;

    // CLONE
    virtual std::unique_ptr<IMediatorEntity>
    Clone(EntityFactory& factory) const = 0;

    // FIND
    IComponent* FindComponent(ComponentType type);
};
