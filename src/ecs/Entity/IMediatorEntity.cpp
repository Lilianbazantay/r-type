#include "IMediatorEntity.hpp"
#include "ecs/IComponent.hpp"

/**
 * @brief returns the actuators components of the entity
 *
 * @return std::vector<IComponent> The list of components
 */
std::vector<IComponent> IMediatorEntity::GetActuatorComponents()
{
    return _actuatorComponents;
};

/**
 * @brief adds a actuator component to the entity
 *
 * @param component The actuator component to add
 */
void IMediatorEntity::AddActuatorComponent(IComponent component)
{
    _actuatorComponents.push_back(component);
};

/**
 * @brief returns the undergoers components of the entity
 *
 * @return std::vector<IComponent> The list of components
 */
std::vector<IComponent> IMediatorEntity::GetUnderGoerComponents()
{
    return _undergoerComponents;
};

/**
 * @brief adds a undergoer component to the entity
 *
 * @param component The undergoer component to add
 */
void IMediatorEntity::AddUndergoerComponent(IComponent component)
{
    _undergoerComponents.push_back(component);
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
        if (_actuatorComponents[i].GetType() == type)
            return &_actuatorComponents[i];
    }
    for (size_t i = 0; i < _undergoerComponents.size(); i++) {
        if (_undergoerComponents[i].GetType() == type)
            return &_undergoerComponents[i];
    }
    return nullptr;
}
