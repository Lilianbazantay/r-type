
#include "MovementSystem.hpp"
#include "../Component/Position.hpp"
#include "../Component/Velocity.hpp"
#include "../Component/Direction.hpp"
#include "../Component/Gravity.hpp"
#include "ecs/IComponent.hpp"
#include <utility>

/**
 * @brief Default constructor of Movement System
*/
MovementSystem::MovementSystem() {
    this->requiedComponents.push_back(ComponentType::POSITION);
    this->requiedComponents.push_back(ComponentType::DIRECTION);

    this->optionnalComponents.push_back(ComponentType::VELOCITY);
    this->optionnalComponents.push_back(ComponentType::GRAVITY);
}

/**
 * @brief Modifies the position of the player based on Velocity, runtime, and gravity
 *
 * @param entity entity getting checked
 * @param data necessary data for systems. See "../relevant_data.hpp" for more information
*/
void MovementSystem::executeEntity(IMediatorEntity &entity, relevant_data &data) {
    Position *playerPos = dynamic_cast<Position*>(entity.FindComponent(ComponentType::POSITION));
    Velocity *playerVelocity = dynamic_cast<Velocity*>(entity.FindComponent(ComponentType::VELOCITY));
    Direction *playerDirection = dynamic_cast<Direction*>(entity.FindComponent(ComponentType::DIRECTION));
    Gravity *playerGravity = dynamic_cast<Gravity*>(entity.FindComponent(ComponentType::GRAVITY));
    std::pair<float, float> newpos = playerPos->GetPosition();
    std::pair<float, float> dir = playerDirection->GetDirection();
    std::pair<float, float> force = {0, 0};

    if (playerVelocity != nullptr) {
        force.first = playerVelocity->GetVelocity();
        force.second = playerVelocity->GetVelocity();
    }
    if (playerGravity != nullptr && playerGravity->IsActivated())
        force.second -= playerGravity->GetAttractionStrength();
    newpos.first += dir.first * force.first * data.runtime;
    newpos.second += dir.second * force.second * data.runtime;
    playerPos->SetPosition(newpos);
}
