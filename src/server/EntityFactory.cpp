#include "EntityFactory.hpp"

#include "../ecs/Component/Position.hpp"
#include "../ecs/Component/Velocity.hpp"
#include "../ecs/Component/Direction.hpp"
#include "../ecs/Component/Hp.hpp"
#include "../ecs/Component/Hitbox.hpp"
#include "../ecs/Component/AnimatedSprite.hpp"
#include "../ecs/Component/Clock.hpp"
#include "../ecs/Component/Cooldown.hpp"
#include "../ecs/Component/EntitySpawner.hpp"
#include "../ecs/Component/Gravity.hpp"
#include "../ecs/Component/Sprite.hpp"
#include "../ecs/Component/Sound.hpp"
#include "../ecs/Component/Strategy.hpp"

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
    if (k == "animatedsprite") return ComponentType::ANIMATED_SPRITE;
    if (k == "clock")     return ComponentType::CLOCK;
    if (k == "cooldown")  return ComponentType::COOLDOWN;
    if (k == "entityspawner") return ComponentType::ENTITY_SPAWNER;
    if (k == "gravity")   return ComponentType::GRAVITY;
    if (k == "sprite")    return ComponentType::SPRITE;
    if (k == "sound")     return ComponentType::SOUND;
    if (k == "strategy")  return ComponentType::STRATEGY;

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
            return std::make_unique<Position>(j[0].get<float>(), j[1].get<float>());
        });

    registerConstructor(ComponentType::VELOCITY,
        [](const json& j) {
            return std::make_unique<Velocity>(j.get<float>());
        });

    registerConstructor(ComponentType::DIRECTION,
        [](const json& j) {
            return std::make_unique<Direction>(
                j[0].get<float>(),
                j[1].get<float>()
            );
        });

    registerConstructor(ComponentType::HP,
        [](const json& j) {
            return std::make_unique<Hp>(j.get<int>());
        });

    registerConstructor(ComponentType::HITBOX,
        [](const json& j) {
            auto size  = j["size"];
            auto layer = j["layer"].get<std::vector<int>>();
            auto mask  = j["mask"].get<std::vector<int>>();
            int damage = j["damage"].get<int>();

            return std::make_unique<Hitbox>(
                size[0].get<float>(),
                size[1].get<float>(),
                damage,
                layer,
                mask
            );
        });

    registerConstructor(ComponentType::ANIMATED_SPRITE,
        [](const json& j) {

            std::unordered_map<std::string, AnimationData> animations;

            for (auto& [name, anim] : j["animations"].items()) {
                AnimationData data;
                data.frameSize = {
                    anim["frame_size"][0].get<int>(),
                    anim["frame_size"][1].get<int>()
                };
                data.start = {
                    anim["start"][0].get<int>(),
                    anim["start"][1].get<int>()
                };
                data.step = {
                    anim["step"][0].get<int>(),
                    anim["step"][1].get<int>()
                };
                data.length = anim["length"].get<int>();
                data.loop = anim.value("loop", true);

                animations.emplace(name, data);
            }

            return std::make_unique<AnimatedSprite>(
                j["spritesheet"].get<std::string>(),
                j["frame_rate"].get<float>(),
                j["default"].get<std::string>(),
                animations
            );
        }
    );



    registerConstructor(ComponentType::CLOCK,
        [](const json& j) {
            if (j.get<bool>())
                return std::make_unique<Clock>();
            return std::unique_ptr<IComponent>(nullptr);
        });

    registerConstructor(ComponentType::COOLDOWN,
        [](const json& j) {
            return std::make_unique<Cooldown>(j.get<double>());
        });

    registerConstructor(ComponentType::ENTITY_SPAWNER,
        [](const json& j) {
            auto mediatorEntity = std::make_unique<MediatorEntity>(
                j["entity_type"].get<std::string>(),
                j["position"][0].get<float>(), j["position"][1].get<float>()
            );
            return std::make_unique<EntitySpawner>(
                j["cooldown"].get<double>(),
                std::move(mediatorEntity),
                j["is_activated"].get<int>()
            );
        });

    registerConstructor(ComponentType::GRAVITY,
        [](const json& j) {
            return std::make_unique<Gravity>(j["strength"].get<float>());
        });

    registerConstructor(ComponentType::SPRITE,
        [](const json& j) {
            return std::make_unique<Sprite>(
                j["file_path"].get<std::string>(),
                j["size_x"].get<int>(), j["size_y"].get<int>()
            );
        });

    registerConstructor(ComponentType::SOUND,
        [](const json& j) {

            std::unordered_map<std::string, SoundData> sounds;

            for (auto& [name, snd] : j["sounds"].items()) {
                SoundData data;
                data.filePath = snd["file_path"].get<std::string>();
                data.volume = snd["volume"].get<float>();
                data.loop = snd["loop"].get<bool>();

                sounds.emplace(name, data);
            }

            return std::make_unique<Sound>(
                j["default"].get<std::string>(),
                sounds
            );
        }
    );


    registerConstructor(ComponentType::STRATEGY,
        [](const json& j) {
            std::vector<Strategy::Pattern> patterns;

            for (const auto& patternJson : j["patterns"]) {
                Strategy::Pattern pattern;

                pattern.hp_max = patternJson["hp_range"][0].get<int>();
                pattern.hp_min = patternJson["hp_range"][1].get<int>();

                for (const auto& seqJson : patternJson["sequences"]) {
                    Strategy::Sequence sequence;

                    for (const auto& commandJson : seqJson) {
                        const auto& actionJson = commandJson[0];

                        int actionX = actionJson[0].get<int>();
                        int actionY = actionJson[1].get<int>();
                        int value   = commandJson[1].get<int>();

                        sequence.emplace_back(
                            std::make_pair(actionX, actionY),
                            value
                        );
                    }

                    pattern.sequences.push_back(std::move(sequence));
                }

                patterns.push_back(std::move(pattern));
            }

            return std::make_unique<Strategy>(patterns);
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

        auto ctorIt = constructors.find(type);
        if (ctorIt == constructors.end())
            continue;

        auto component = ctorIt->second(value);
        if (component)
            entity.AddActuatorComponent(std::move(component));

    }

    return true;
}
