#include "IMediatorEntity.hpp"
#include "../IComponent.hpp"
#include <iostream>

/**
 * @brief returns the actuators components of the entity
 *
 * @return std::vector<IComponent> The list of components
 */
std::vector<ComponentType> IMediatorEntity::GetActuatorComponents()
{
    return _actuatorTypeList;
};

/**
 * @brief adds a actuator component to the entity
 *
 * @param component The actuator component to add
 */
void IMediatorEntity::AddActuatorComponent(std::unique_ptr<IComponent> component)
{
    _actuatorTypeList.push_back(component->GetType());
    _actuatorComponents.push_back(std::move(component));
};

/**
 * @brief returns the undergoers components of the entity
 *
 * @return std::vector<IComponent> The list of components
 */
std::vector<ComponentType> IMediatorEntity::GetUnderGoerComponents()
{
    return _undergoerTypeList;
};

/**
 * @brief adds a undergoer component to the entity
 *
 * @param component The undergoer component to add
 */
void IMediatorEntity::AddUndergoerComponent(std::unique_ptr<IComponent> component)
{
    _undergoerTypeList.push_back(component->GetType());
    _undergoerComponents.push_back(std::move(component));
};

/**
 * @brief Return the adresse of the Component with the type given in parametter
 *
 * @param type Component searched
 * @return IComponent* Return the addre of the component searcher or nullptr if there is no component
 */
IComponent *IMediatorEntity::FindComponent(ComponentType type)
{
    for (size_t i = 0; i < _actuatorComponents.size(); i++) {
        if (_actuatorComponents[i]->GetType() == type)
            return _actuatorComponents[i].get();
    }
    for (size_t i = 0; i < _undergoerComponents.size(); i++) {
        if (_undergoerComponents[i]->GetType() == type)
            return _undergoerComponents[i].get();
    }
    std::cout << "unfound" << std::endl;
    return nullptr;
}

void IMediatorEntity::setId(int newId) {
    id = newId;
}

int IMediatorEntity::getId() {
    return id;
}

void IMediatorEntity::setType(int newType) {
    Entitytype = newType;
}

int IMediatorEntity::getType() {
    return Entitytype;
}

/**
 * @brief checks if the entity matches with the type and id
 *
 * @param _type type of the entity
 * @param _id personnal entity ID
*/
bool IMediatorEntity::is_wanted_entity(int _type, int _id) {
    if (_id == id && _type == Entitytype)
        return true;
    return false;
}

void IMediatorEntity::lock() {
    _mutex.lock();
}

void IMediatorEntity::unlock() {
    _mutex.unlock();
}

/**
 * @brief
 *
 *
 */
bool IMediatorEntity::justCreated() {
    if (entity_created) {
        entity_created = false;
        return true;
    }
    return false;
}

bool IMediatorEntity::hasChanged() {
    if (entity_changed) {
        entity_changed = false;
        return true;
    }
    return false;
}

void IMediatorEntity::hasChanged(bool val) {
    entity_changed = val;
}

bool IMediatorEntity::is_Alive() {
    return entity_live;
}

void IMediatorEntity::Alive(bool val) {
    entity_live = val;
}
