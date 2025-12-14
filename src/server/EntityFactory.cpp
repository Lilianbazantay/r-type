#include "EntityFactory.hpp"

#include "../ecs/Component/Position.hpp"
#include "../ecs/Component/Velocity.hpp"
#include "../ecs/Component/Direction.hpp"
#include "../ecs/Component/Hp.hpp"
#include "../ecs/Component/Hitbox.hpp"
#include "../ecs/Component/Attack.hpp"

#include <iostream>
#include <algorithm>

using json = nlohmann::json;

/**
 * @brief Registers a map of prototypes in the factory.
 *
 * @param protos Map of prototype name to Prototype objects
 */
void EntityFactory::registerPrototypes(
    const std::unordered_map<std::string, Prototype>& protos)
{
    prototypes = protos;
}

/**
 * @brief Registers a component constructor function for a given type.
 *
 * @param type The component type
 * @param fn The constructor function
 */
void EntityFactory::registerConstructor(ComponentType type, ConstructorFn fn)
{
    constructors[type] = std::move(fn);
}

/**
 * @brief Converts a string key to a ComponentType enum.
 *
 * @param key The string representation of the component
 * @return ComponentType The corresponding ComponentType, or UNKNOWN if invalid
 */
ComponentType EntityFactory::stringToComponentType(const std::string &key)
{
    std::string k = key;
    std::transform(k.begin(), k.end(), k.begin(), ::tolower);

    if (k == "position") return ComponentType::POSITION;
    if (k == "velocity") return ComponentType::VELOCITY;
    if (k == "direction") return ComponentType::DIRECTION;
    if (k == "hp")        return ComponentType::HP;
    if (k == "hitbox")    return ComponentType::HITBOX;
    if (k == "attack")    return ComponentType::ATTACK;

    return ComponentType::UNKNOWN;
}

/**
 * @brief Registers the default constructors for all supported component types.
 *
 */
void EntityFactory::registerComponentConstructors()
{
    registerConstructor(ComponentType::POSITION,
        [](const json& j) {
            return std::make_unique<Position>(j[0], j[1]);
        });

    registerConstructor(ComponentType::VELOCITY,
        [](const json& j) {
            return std::make_unique<Velocity>(j.get<float>());
        });

    registerConstructor(ComponentType::HP,
        [](const json& j) {
            return std::make_unique<Hp>(j.get<int>());
        });

    registerConstructor(ComponentType::HITBOX,
        [](const json& j) {
            auto size = j["size"];
            auto layer = j["layer"].get<std::vector<int>>();
            auto mask  = j["mask"].get<std::vector<int>>();
            return std::make_unique<Hitbox>(
                size[0], size[1], 1, layer, mask
            );
        });

    registerConstructor(ComponentType::ATTACK,
        [](const json& j) {
            return std::make_unique<Attack>(
                j["damage"], j["fireRate"]
            );
        });
}

/**
 * @brief Applies a prototype to an entity by adding its components.
 *
 * @param protoName Name of the prototype to apply
 * @param entity The entity to which the prototype will be applied
 * @return true if the prototype was applied successfully
 * @return false if the prototype does not exist or cannot be applied
 */
bool EntityFactory::applyPrototypeToEntity(
    const std::string &protoName,
    IMediatorEntity &entity)
{
    auto it = prototypes.find(protoName);
    if (it == prototypes.end()) {
        std::cerr << "[Factory] Unknown prototype: " << protoName << "\n";
        return false;
    }

    const json& root = it->second.data;

    if (!root.contains("components"))
        return false;

    const json& components = root["components"];

    for (auto &[key, value] : components.items()) {
        ComponentType type = stringToComponentType(key);
        if (type == ComponentType::UNKNOWN)
            continue;

        auto ctor = constructors.find(type);
        if (ctor == constructors.end())
            continue;

        entity.AddActuatorComponent(ctor->second(value));
    }

    return true;
}
