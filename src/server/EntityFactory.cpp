#include "EntityFactory.hpp"
#include "../ecs/Entity/IMediatorEntity.hpp"
#include "../ecs/Entity/Entities.hpp"

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

#include <cstddef>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

void EntityFactory::registerPrototypes(
    const std::unordered_map<std::string, Prototype>& protos)
{
    prototypes = protos;
}

void EntityFactory::registerConstructor(ComponentType type, ConstructorFn fn)
{
    constructors[type] = std::move(fn);
}

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

static std::unique_ptr<IMediatorEntity> createEntityFromType(EntityFactory& factory, const std::string& type) {
    if (type == "Player") return std::make_unique<Player>(factory);
    if (type == "Enemy") return std::make_unique<Enemy>(factory);
    if (type == "PlayerBullet") return std::make_unique<PlayerBullet>(factory);
    if (type == "EnemyBullet") return std::make_unique<EnemyBullet>(factory);
    if (type == "Background") return std::make_unique<Background>(factory);
    if (type == "Wall1") return std::make_unique<Wall1>(factory);
    if (type == "Wall2") return std::make_unique<Wall2>(factory);
    if (type == "Wall3") return std::make_unique<Wall3>(factory);
    if (type == "Wall4") return std::make_unique<Wall4>(factory);

    std::cerr << "[Factory] Unknown entity type: " << type << "\n";
    return nullptr;
}

static ANIMATION_TYPE stringToAnimationType(const std::string& str) {
    std::string s = str;
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);

    if (s == "IDLE") return IDLE;
    if (s == "UP") return UP;
    if (s == "DOWN") return DOWN;
    if (s == "SHOOT") return SHOOT;
    if (s == "DEATH") return DEATH;

    return NOTHING;
}

void EntityFactory::registerComponentConstructors()
{
    registerConstructor(ComponentType::POSITION,
        [](const json& j) {
            return std::make_unique<Position>(
                static_cast<float>(j[0].get<double>()),
                static_cast<float>(j[1].get<double>())
            );
        });

    registerConstructor(ComponentType::VELOCITY,
        [](const json& j) {
            return std::make_unique<Velocity>(static_cast<float>(j.get<double>()));
        });

    registerConstructor(ComponentType::DIRECTION,
        [](const json& j) {
            return std::make_unique<Direction>(
                static_cast<float>(j[0].get<double>()),
                static_cast<float>(j[1].get<double>())
            );
        });

    registerConstructor(ComponentType::HP,
        [](const json& j) {
            return std::make_unique<Hp>(static_cast<int>(j.get<int>()));
        });

    registerConstructor(ComponentType::HITBOX,
        [](const json& j) {
            auto size  = j["size"];
            auto layer = j["layer"].get<std::vector<int>>();
            auto mask  = j["mask"].get<std::vector<int>>();
            int damage = j["damage"].get<int>();

            return std::make_unique<Hitbox>(
                static_cast<float>(size[0].get<double>()),
                static_cast<float>(size[1].get<double>()),
                damage,
                layer,
                mask
            );
        });

    registerConstructor(ComponentType::ANIMATED_SPRITE,
        [](const json& j) {
            auto sprite = std::make_unique<AnimatedSprite>(
                j["spritesheet"].get<std::string>(),
                std::make_pair(1.f, 1.f),
                std::make_pair(
                    static_cast<float>(j["animations"].begin().value()["frame_size"][0].get<double>()),
                    static_cast<float>(j["animations"].begin().value()["frame_size"][1].get<double>())
                )
            );

            sprite->setAnimationRate(static_cast<float>(j["frame_rate"].get<double>()));

            for (auto& [name, anim] : j["animations"].items()) {
                ANIMATION_TYPE type = stringToAnimationType(name);
                if (type == NOTHING)
                    continue;

                sprite->addAnimation(
                    { static_cast<float>(anim["frame_size"][0].get<double>()), static_cast<float>(anim["frame_size"][1].get<double>()) },
                    { static_cast<float>(anim["start"][0].get<double>()), static_cast<float>(anim["start"][1].get<double>()) },
                    { static_cast<float>(anim["step"][0].get<double>()), static_cast<float>(anim["step"][1].get<double>()) },
                    type,
                    anim.value("loop", true),
                    anim["length"].get<int>()
                );
            }

            sprite->changeAnimation(
                stringToAnimationType(j["default"].get<std::string>()),
                false
            );

            return sprite;
        }
    );

    registerConstructor(ComponentType::CLOCK,
        [](const json& j) {
            return std::make_unique<Clock>();
        });

    registerConstructor(ComponentType::COOLDOWN,
        [](const json& j) {
            return std::make_unique<Cooldown>(static_cast<float>(j.get<double>()));
        });

    registerConstructor(ComponentType::ENTITY_SPAWNER,
        [this](const json& j) {
            auto mediatorEntity = createEntityFromType(*this, j["entity_type"].get<std::string>());

            if (!mediatorEntity) {
                std::cerr << "[Factory] Failed to create entity of type " << j["entity_type"] << "\n";
                return std::unique_ptr<EntitySpawner>(nullptr);
            }

            mediatorEntity->AddActuatorComponent(std::make_unique<Position>(
                static_cast<float>(j["position"][0].get<double>()),
                static_cast<float>(j["position"][1].get<double>())
            ));

            return std::make_unique<EntitySpawner>(
                static_cast<float>(j["cooldown"].get<double>()),
                std::move(mediatorEntity),
                *this,
                j["is_activated"].get<int>()
            );
        }
    );

    registerConstructor(ComponentType::GRAVITY,
        [](const json& j) {
            return std::make_unique<Gravity>(true, static_cast<float>(j["strength"].get<double>()));
        });

    registerConstructor(ComponentType::SPRITE,
        [](const json& j) {
            return std::make_unique<Sprite>(
                j["file_path"].get<std::string>(),
                static_cast<int>(j["size"][0].get<double>()),
                static_cast<int>(j["size"][1].get<double>())
            );
        });

    registerConstructor(ComponentType::STRATEGY,
        [](const json& j) {

            auto strategy = std::make_unique<Strategy>(std::vector<std::pair<std::pair<float, float>, float>>{});

            for (const auto& patternJson : j["patterns"]) {

                int hpPercent = static_cast<int>(patternJson["hp_range"][1].get<double>());

                if (!patternJson.contains("sequences") ||
                    patternJson["sequences"].empty())
                    continue;

                for (const auto& sequence : patternJson["sequences"]) {
                    for (const auto& commandJson : sequence) {
                        const auto& dir = commandJson[0];

                        float dx = static_cast<float>(dir[0].get<double>());
                        float dy = static_cast<float>(dir[1].get<double>());
                        float time = static_cast<float>(commandJson[1].get<double>());
                        std::cout << dx << "; " << dy << "; " << time << "\n";

                        strategy->AddElementToPattern(
                            {dx, dy},
                            time,
                            hpPercent
                        );
                    }
                }
            }
            return strategy;
        });
}

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
