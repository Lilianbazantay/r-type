#include "IMediatorEntity.hpp"

/**
 * @brief returns the actuators components of the entity
 *
 * @return std::vector<IComponent> The list of components
 */
std::vector<IComponent> IMediatorEntity::GetActuatorComponents()
{
    return actuatorComponents;
};

/**
 * @brief adds a actuator component to the entity
 *
 * @param component The actuator component to add
 */
void IMediatorEntity::AddActuatorComponent(IComponent component)
{
    actuatorComponents.push_back(component);
};

/**
 * @brief returns the undergoers components of the entity
 *
 * @return std::vector<IComponent> The list of components
 */
std::vector<IComponent> IMediatorEntity::GetUnderGoerComponents()
{
    return undergoerComponents;
};

/**
 * @brief adds a undergoer component to the entity
 *
 * @param component The undergoer component to add
 */
void IMediatorEntity::AddUndergoerComponent(IComponent component)
{
    undergoerComponents.push_back(component);
};