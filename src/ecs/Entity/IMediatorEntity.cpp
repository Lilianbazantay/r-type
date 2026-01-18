#include "IMediatorEntity.hpp"

/**
 * @brief returns the actuators components of the entity
 */
std::vector<ComponentType> IMediatorEntity::GetActuatorComponents() const
{
    return _actuatorTypeList;
}

/**
 * @brief adds an actuator component to the entity
 */
void IMediatorEntity::AddActuatorComponent(std::unique_ptr<IComponent> component)
{
    _actuatorTypeList.push_back(component->GetType());
    _actuatorComponents.push_back(std::move(component));
}

/**
 * @brief returns the undergoers components of the entity
 */
std::vector<ComponentType> IMediatorEntity::GetUnderGoerComponents() const
{
    return _undergoerTypeList;
}

/**
 * @brief adds an undergoer component to the entity
 */
void IMediatorEntity::AddUndergoerComponent(std::unique_ptr<IComponent> component)
{
    _undergoerTypeList.push_back(component->GetType());
    _undergoerComponents.push_back(std::move(component));
}

/**
 * @brief Find a component by type
 */
IComponent* IMediatorEntity::FindComponent(ComponentType type)
{
    for (auto& comp : _actuatorComponents) {
        if (comp->GetType() == type)
            return comp.get();
    }
    for (auto& comp : _undergoerComponents) {
        if (comp->GetType() == type)
            return comp.get();
    }
    return nullptr;
}

void IMediatorEntity::setId(int newId)
{
    id = newId;
}

int IMediatorEntity::getId() const
{
    return id;
}

void IMediatorEntity::setType(int newType)
{
    Entitytype = newType;
}

int IMediatorEntity::getType() const
{
    return Entitytype;
}

bool IMediatorEntity::is_wanted_entity(int type, int _id) const
{
    return (_id == id && type == Entitytype);
}

void IMediatorEntity::lock()
{
    _mutex.lock();
}

void IMediatorEntity::unlock()
{
    _mutex.unlock();
}

bool IMediatorEntity::justCreated()
{
    if (entity_created) {
        entity_created = false;
        return true;
    }
    return false;
}

bool IMediatorEntity::hasChanged()
{
    if (entity_changed) {
        entity_changed = false;
        return true;
    }
    return false;
}

void IMediatorEntity::hasChanged(bool val)
{
    entity_changed = val;
}

bool IMediatorEntity::is_Alive() const
{
    return entity_live;
}

void IMediatorEntity::Alive(bool val)
{
    entity_live = val;
}
