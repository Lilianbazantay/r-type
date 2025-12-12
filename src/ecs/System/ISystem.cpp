
#include "ISystem.hpp"
#include "../Entity/IMediatorEntity.hpp"
#include "../IComponent.hpp"
#include <iostream>
#include <vector>

/**
 * @brief Checks if component requirements are met for an entity
 *
 * @param entity main entity
 *
 * @return true if requirements are met
 * @return false if not every requirement is met
*/
bool ISystem::checkRequirements(IMediatorEntity& entity) {
    std::vector<ComponentType> entityComponents = entity.GetActuatorComponents();
    int requirements = this->requiedComponents.size();
    ComponentType rType;

    for (size_t i = 0; i < this->requiedComponents.size(); i++) {
        rType = this->requiedComponents[i];
        for (size_t j = 0; j < entityComponents.size(); j++)
            if (entityComponents[j] == rType) {
                requirements--;
                j = entityComponents.size();
            }
    }
    if (requirements == 0)
        return true;
    return false;
}

/**
 * @brief add a list of optionnal component types. Not counted as requirements
*/
void ISystem::addRequiredComponentType(std::shared_ptr<IComponent> newComponent) {
    this->requiedComponents.push_back(newComponent->GetType());
}

/**
 * @brief add an required component type. Counted as requirements
*/
void ISystem::addRequiredComponentType(ComponentType type) {
    this->requiedComponents.push_back(type);
}

/**
 * @brief add a list of optionnal component types. Not counted as requirements
*/
void ISystem::addRequiredComponentType(std::vector<ComponentType> typeList) {
    for (size_t i = 0; i < typeList.size(); i++)
        this->requiedComponents.push_back(typeList[i]);
}

/**
 * @brief add an optionnal component type. Not counted as requirements
*/
void ISystem::addOptionnalComponentType(ComponentType type) {
    this->optionnalComponents.push_back(type);
}



/**
 * @brief calls requirements check, then if applicable applies the system execution to it
 *
 * @param entity entity getting checked
 * @param data necessary data for systems. See "../relevant_data.hpp" for more information
*/
void ISystem::checkEntity(IMediatorEntity& entity, relevant_data_t& data) {
    std::cout << "Checkin" << std::endl;
    if (checkRequirements(entity) == false)
        return;
    executeEntity(entity, data);
}
