
#include "MovementSystem.hpp"

MovementSystem::MovementSystem() {
    this->requiedComponents.push_back(ComponentType::POSITION);

    this->optionnalComponents.push_back(ComponentType::VELOCITY);
    this->optionnalComponents.push_back(ComponentType::GRAVITY);
}

void MovementSystem::executeEntity(IMediatorEntity &entity) {

}
