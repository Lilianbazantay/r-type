#pragma once
#include "EntityFactory.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/relevant_data.hpp"
#include <SFML/System/Clock.hpp>
#include <memory>
#include <vector>
#include <functional>

class EntityPersonnalization {
private:
    int entityType = ENTITY_ENEMY;
    float creationTimer = 0.0f;
    EntityFactory& factory;

public:
    explicit EntityPersonnalization(EntityFactory& factoryRef);
    EntityPersonnalization(
        EntityFactory& factoryRef,
        int entityType,
        float timer
    );

    // non copiable
    EntityPersonnalization(const EntityPersonnalization&) = delete;
    EntityPersonnalization& operator=(const EntityPersonnalization&) = delete;

    // movable
    EntityPersonnalization(EntityPersonnalization&&) = default;
    EntityPersonnalization& operator=(EntityPersonnalization&&) = default;

    ~EntityPersonnalization() = default;

    void setType(int);
    void setCreationTimer(float);

    float getCreationTimer() const;
    int getType() const;

    // supprime getEntity() ici, on utilisera createEntity via callback
};

class WaveManager {
private:
    sf::Clock clock;

    std::vector<std::unique_ptr<EntityPersonnalization>> waveEntities;
    std::vector<std::vector<std::unique_ptr<EntityPersonnalization>>> entityWaveCreationArray;

    EntityFactory& factory;

    // Callback pour créer des entités via ServerGame
    std::function<void(int type, relevant_data_t* data)> createEntityCallback;

public:
    explicit WaveManager(EntityFactory& factoryRef);

    void setCreateEntityCallback(std::function<void(int, relevant_data_t*)> cb) {
        createEntityCallback = cb;
    }

    void computeEntities(relevant_data_t*);
    void setWave(size_t);
    void addWave(std::vector<std::unique_ptr<EntityPersonnalization>> wave);
};
