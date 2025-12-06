
#include "ISystem.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/IComponent.hpp"
#include <vector>


bool ISystem::checkRequirements(IMediatorEntity& entity) {
    std::vector<IComponent> entityComponents = entity.GetActuatorComponents();
    int requirements = this->requiedComponents.size();
    ComponentType rType;

    for (size_t i = 0; i < this->requiedComponents.size(); i++) {
        rType = this->requiedComponents[i];
        for (size_t j = 0; j < entityComponents.size(); j++)
            if (entityComponents[j].GetType() == rType) {
                requirements--;
                j = entityComponents.size();
            }
    }
    if (requirements == 0)
        return true;
    return false;
}

void ISystem::addRequiredComponentType(std::shared_ptr<IComponent> newComponent) {
    this->requiedComponents.push_back(newComponent->GetType());
}

void ISystem::addRequiredComponentType(ComponentType type) {
    this->requiedComponents.push_back(type);
}

void ISystem::addRequiredComponentType(std::vector<ComponentType> typeList) {
    for (size_t i = 0; i < typeList.size(); i++)
        this->requiedComponents.push_back(typeList[i]);
}

void ISystem::addOptionnalComponentType(ComponentType type) {
    this->optionnalComponents.push_back(type);
}



void ISystem::checkEntity(IMediatorEntity& entity) {
    if (checkRequirements(entity) == false)
        return;
    executeEntity(entity);
}
