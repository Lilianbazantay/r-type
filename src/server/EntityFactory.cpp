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
using ComponentPtr = std::shared_ptr<IComponent>;

void EntityFactory::registerPrototypes(const std::unordered_map<std::string, Prototype>& protos) {
    prototypes = protos;
}

void EntityFactory::registerConstructor(ComponentType type, ConstructorFn fn) {
    constructors[type] = fn;
}

ComponentType EntityFactory::stringToComponentType(const std::string &key) {
    std::string k = key;
    std::transform(k.begin(), k.end(), k.begin(), [](unsigned char c){ return std::tolower(c); });

    if (k == "hp" || k == "health") return ComponentType::HP;
    if (k == "position") return ComponentType::POSITION;
    if (k == "velocity" || k == "speed") return ComponentType::VELOCITY;
    if (k == "direction") return ComponentType::DIRECTION;
    if (k == "hitbox") return ComponentType::HITBOX;
    if (k == "attack" || k == "weapon" || k == "stats") return ComponentType::ATTACK;

    return ComponentType::UNKNOWN;
}

void EntityFactory::registerComponentConstructors() {

    registerConstructor(ComponentType::POSITION, [](const json &j)->ComponentPtr {
        float x = 0.f, y = 0.f;
        if (j.is_array() && j.size() >= 2) {
            x = j[0].get<float>();
            y = j[1].get<float>();
        } else if (j.is_object()) {
            if (j.contains("x")) x = j["x"].get<float>();
            if (j.contains("y")) y = j["y"].get<float>();
            if (j.contains("width") && j.contains("height")) {
                x = j["x"].get<float>();
                y = j["y"].get<float>();
            }
        } else if (j.is_string()) {
            std::string s = j.get<std::string>();
            auto comma = s.find(',');
            if (comma != std::string::npos) {
                x = std::stof(s.substr(0, comma));
                y = std::stof(s.substr(comma + 1));
            }
        } else if (j.is_number()) {
            x = j.get<float>();
            y = 0.f;
        }
        return std::make_shared<Position>(x, y);
    });

    registerConstructor(ComponentType::VELOCITY, [](const json &j)->ComponentPtr {
        float v = 0.f;
        if (j.is_number()) v = j.get<float>();
        else if (j.is_string()) v = std::stof(j.get<std::string>());
        else if (j.is_object() && j.contains("speed")) v = j["speed"].get<float>();
        return std::make_shared<Velocity>(v);
    });

    registerConstructor(ComponentType::DIRECTION, [](const json &j)->ComponentPtr {
        if (j.is_number()) {
            return std::make_shared<Direction>(j.get<float>());
        } else if (j.is_array() && j.size() >= 2) {
            return std::make_shared<Direction>(j[0].get<float>(), j[1].get<float>());
        } else if (j.is_string()) {

            std::string s = j.get<std::string>();
            auto comma = s.find(',');
            if (comma != std::string::npos) {
                float x = std::stof(s.substr(0, comma));
                float y = std::stof(s.substr(comma + 1));
                return std::make_shared<Direction>(x, y);
            } else {
                return std::make_shared<Direction>(std::stof(s));
            }
        }
        return std::make_shared<Direction>(0.f);
    });

    registerConstructor(ComponentType::HP, [](const json &j)->ComponentPtr {
        int hp = 0;
        if (j.is_number_integer()) hp = j.get<int>();
        else if (j.is_object()) {
            if (j.contains("health")) hp = j["health"].get<int>();
            else if (j.contains("hp")) hp = j["hp"].get<int>();
        } else if (j.is_string()) hp = std::stoi(j.get<std::string>());
        return std::make_shared<Hp>(hp);
    });

    registerConstructor(ComponentType::HITBOX, [](const json &j)->ComponentPtr {
        float sx = 0.f, sy = 0.f;
        std::vector<int> layers;
        std::vector<int> masks;

        if (j.is_array() && j.size() >= 2) {
            sx = j[0].get<float>();
            sy = j[1].get<float>();
        } else if (j.is_object()) {
            if (j.contains("size")) {
                auto s = j["size"];
                if (s.is_object()) {
                    if (s.contains("width")) sx = s["width"].get<float>();
                    if (s.contains("height")) sy = s["height"].get<float>();
                } else if (s.is_array() && s.size() >= 2) {
                    sx = s[0].get<float>();
                    sy = s[1].get<float>();
                }
            }
            if (j.contains("layer") && j["layer"].is_array()) {
                for (auto &v : j["layer"]) layers.push_back(v.get<int>());
            } else if (j.contains("layers") && j["layers"].is_array()) {
                for (auto &v : j["layers"]) layers.push_back(v.get<int>());
            }
            if (j.contains("mask") && j["mask"].is_array()) {
                for (auto &v : j["mask"]) masks.push_back(v.get<int>());
            } else if (j.contains("masks") && j["masks"].is_array()) {
                for (auto &v : j["masks"]) masks.push_back(v.get<int>());
            }
        } else if (j.is_string()) {
            std::string s = j.get<std::string>();
            auto comma = s.find(',');
            if (comma != std::string::npos) {
                sx = std::stof(s.substr(0, comma));
                sy = std::stof(s.substr(comma + 1));
            }
        }
        if (layers.empty() && masks.empty()) {
            return std::make_shared<Hitbox>(sx, sy);
        }
        return std::make_shared<Hitbox>(sx, sy, layers, masks);
    });

    registerConstructor(ComponentType::ATTACK, [](const json &j)->ComponentPtr {
        float dmg = 0.f;
        float fr = 0.f;
        if (j.is_array()) {
            if (j.size() >= 1) dmg = j[0].get<float>();
            if (j.size() >= 2) fr = j[1].get<float>();
        } else if (j.is_object()) {
            if (j.contains("damage")) dmg = j["damage"].get<float>();
            if (j.contains("fireRate")) fr = j["fireRate"].get<float>();
            if (j.contains("firerate")) fr = j["firerate"].get<float>();
        } else if (j.is_number()) {
            dmg = j.get<float>();
        } else if (j.is_string()) {
            std::string s = j.get<std::string>();
            auto comma = s.find(',');
            if (comma != std::string::npos) {
                dmg = std::stof(s.substr(0, comma));
                fr  = std::stof(s.substr(comma + 1));
            } else {
                dmg = std::stof(s);
            }
        }
        return std::make_shared<Attack>(dmg, fr);
    });
}

int EntityFactory::createEntityFromPrototype(const std::string &protoName, IMediatorEntity &mediator) {
    auto it = prototypes.find(protoName);
    if (it == prototypes.end()) {
        std::cerr << "EntityFactory::createEntityFromPrototype - prototype not found: " << protoName << "\n";
        return -1;
    }

    const Prototype &proto = it->second;
    int entityId = mediator.createEntity(proto.name);

    if (!proto.data.is_object()) {

        return entityId;
    }

    for (auto it = proto.data.begin(); it != proto.data.end(); ++it) {
        const std::string key = it.key();
        const json &value = it.value();

        ComponentType ctype = stringToComponentType(key);

        if (ctype == ComponentType::UNKNOWN) {

            if (value.is_object()) {
                for (auto subIt = value.begin(); subIt != value.end(); ++subIt) {
                    std::string subKey = subIt.key();
                    ComponentType subType = stringToComponentType(subKey);
                    if (subType != ComponentType::UNKNOWN) {
                        auto ctorIt = constructors.find(subType);
                        if (ctorIt != constructors.end()) {
                            ComponentPtr comp = ctorIt->second(subIt.value());

                            mediator.AddActuatorComponent(comp);
                        }
                    }
                }
            }
            continue;
        }

        auto ctorIt = constructors.find(ctype);
        if (ctorIt == constructors.end()) {
            std::cerr << "EntityFactory::createEntityFromPrototype - no constructor for type: " << (int)ctype << " (key='" << key << "')\n";
            continue;
        }

        ComponentPtr comp = ctorIt->second(value);

        mediator.AddActuatorComponent(comp);
    }

    return entityId;
}
