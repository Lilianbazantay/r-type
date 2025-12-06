
#include "ISystem.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/IComponent.hpp"
#include <vector>


bool ISystem::checkRequirements(IMediatorEntity& entity) {
    std::vector<IComponent> entityComponents = entity.GetActuatorComponents();
    int requirements = this->requiedComponents.size();
    ComponentType rType;

    for (int i = 0; i < this->requiedComponents.size(); i++) {
        rType = this->requiedComponents[i];
        for (int j = 0; j < entityComponents.size(); j++)
            if (entityComponents[j].GetType() == rType) {
                requirements--;
                j = entityComponents.size();
            }
    }
    if (requirements == 0)
        return true;
    return false;
}

void ISystem::addRequiredComponent(std::shared_ptr<IComponent> newComponent) {
    this->requiedComponents.push_back(newComponent->GetType());
};

void ISystem::checkEntity(IMediatorEntity& entity) {
    if (checkRequirements(entity) == false)
        return;
    executeEntity(entity);
}
