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

int main()
{
    std::cout << "==== CONFIG / FACTORY TEST ====\n";

    // 1️⃣ Load config
    ConfigParser parser;
    if (!parser.loadFile("configuration/player/player.json")) {
        std::cerr << "❌ Failed to load config\n";
        return 1;
    }
    std::cout << "✔ Config loaded\n";

    // 2️⃣ Register factory
    EntityFactory factory;
    factory.registerPrototypes(parser.getPrototypes());
    factory.registerComponentConstructors();
    std::cout << "✔ Factory ready\n";

    // 3️⃣ Create entity
    TestEntity entity;

    // 4️⃣ Apply prototype
    if (!factory.applyPrototypeToEntity("player_basic", entity)) {
        std::cerr << "❌ Failed to apply prototype\n";
        return 1;
    }
    std::cout << "✔ Prototype applied\n\n";

    // 5️⃣ Print components
    std::cout << "---- ACTUATOR COMPONENTS ----\n";
    for (auto t : entity.GetActuatorComponents()) {
        std::cout << "- " << compToString(t) << "\n";
    }

    // std::cout << "\n---- VALUE CHECK ----\n";

    // if (auto* pos = dynamic_cast<Position*>(
    //     entity.FindComponent(ComponentType::POSITION))) {
    //     std::cout << "Position: (" << pos->x << ", " << pos->y << ")\n";
    // }

    // if (auto* vel = dynamic_cast<Velocity*>(
    //     entity.FindComponent(ComponentType::VELOCITY))) {
    //     std::cout << "Velocity: " << vel->value << "\n";
    // }

    // if (auto* hp = dynamic_cast<Hp*>(
    //     entity.FindComponent(ComponentType::HP))) {
    //     std::cout << "Hp: " << hp->value << "\n";
    // }

    std::cout << "\n==== TEST DONE ====\n";
    return 0;
}
