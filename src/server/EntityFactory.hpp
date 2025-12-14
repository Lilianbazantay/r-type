#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

#include "Prototype.hpp"
#include "../ecs/IComponent.hpp"
#include "../ecs/Entity/IMediatorEntity.hpp"

class EntityFactory {
public:
    using json = nlohmann::json;
    using ComponentUPtr = std::unique_ptr<IComponent>;
    using ConstructorFn = std::function<ComponentUPtr(const json&)>;

    EntityFactory() = default;
    ~EntityFactory() = default;

    void registerPrototypes(const std::unordered_map<std::string, Prototype>& protos);
    void registerComponentConstructors();

    bool applyPrototypeToEntity(const std::string &protoName, IMediatorEntity &entity);

    void registerConstructor(ComponentType type, ConstructorFn fn);

private:
    std::unordered_map<std::string, Prototype> prototypes;
    std::unordered_map<ComponentType, ConstructorFn> constructors;

    static ComponentType stringToComponentType(const std::string &key);
};
