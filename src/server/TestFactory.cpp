#include <iostream>
#include <cassert>

#include "../ecs/Component/Position.hpp"
#include "../ecs/Component/Velocity.hpp"
#include "../ecs/Component/Hp.hpp"
#include "../ecs/Component/Hitbox.hpp"
#include "../ecs/Component/Attack.hpp"
#include "../ecs/Component/Direction.hpp"

#include "ConfigParser.hpp"
#include "EntityFactory.hpp"
#include "../ecs/Entity/IMediatorEntity.hpp"

// Dummy entity for test (no gameplay logic)
class TestEntity : public IMediatorEntity {
public:
    void run() override {}
    IMediatorEntity* Clone() override { return nullptr; }
};

static const char* compToString(ComponentType t)
{
    switch (t) {
        case ComponentType::POSITION: return "Position";
        case ComponentType::VELOCITY: return "Velocity";
        case ComponentType::HP:       return "Hp";
        case ComponentType::HITBOX:   return "Hitbox";
        case ComponentType::ATTACK:   return "Attack";
        case ComponentType::DIRECTION:return "Direction";
        default: return "Unknown";
    }
}
