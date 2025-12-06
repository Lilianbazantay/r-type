
#include "CollisionSystem.hpp"

CollisionSystem::CollisionSystem() {
    this->requiedComponents.push_back(ComponentType::POSITION);
    this->requiedComponents.push_back(ComponentType::HITBOX);
}

void CollisionSystem::executeEntity(IMediatorEntity &entity) {

}
