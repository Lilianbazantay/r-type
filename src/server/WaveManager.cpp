#include "WaveManager.hpp"
#include <iostream>

// ==========================
// EntityPersonnalization
// ==========================

EntityPersonnalization::EntityPersonnalization(EntityFactory& factoryRef)
    : factory(factoryRef) {}

EntityPersonnalization::EntityPersonnalization(
    EntityFactory& factoryRef,
    int type,
    float timer
)
    : entityType(type),
      creationTimer(timer),
      factory(factoryRef) {}

void EntityPersonnalization::setType(int type) {
    entityType = type;
}

void EntityPersonnalization::setCreationTimer(float timer) {
    creationTimer = timer;
}

float EntityPersonnalization::getCreationTimer() const {
    return creationTimer;
}

int EntityPersonnalization::getType() const {
    return entityType;
}

// std::unique_ptr<IMediatorEntity> EntityPersonnalization::getEntity() const {
//     std::cerr << "[DEBUG][WaveManager] getEntity type = " << entityType << "\n";

//     switch (entityType) {
//         case ENTITY_ENEMY:
//             return std::make_unique<Enemy>(factory);
//         case ENTITY_BULLET:
//             return std::make_unique<EnemyBullet>(factory);
//         default:
//             std::cerr << "[ERROR][WaveManager] Unknown entity type\n";
//             return nullptr;
//     }
// }

// ==========================
// WaveManager
// ==========================

WaveManager::WaveManager(EntityFactory& factoryRef)
    : factory(factoryRef)
{
    std::vector<std::unique_ptr<EntityPersonnalization>> wave0;

    for (size_t i = 0; i < 7; ++i) {
        wave0.push_back(
            std::make_unique<EntityPersonnalization>(
                factory,
                ENTITY_ENEMY,
                i * 3.0f
            )
        );
    }

    addWave(std::move(wave0));
    setWave(0);
}

void WaveManager::computeEntities(relevant_data_t* data) {
    if (!data) return;
    if (waveEntities.empty()) return;

    while (!waveEntities.empty() &&
           waveEntities.front()->getCreationTimer()
               <= clock.getElapsedTime().asSeconds()) {

        int entityType = waveEntities.front()->getType();
        waveEntities.erase(waveEntities.begin());

        std::cerr << "[DEBUG][WaveManager] Spawning entity type=" << entityType << "\n";

        if (!createEntityCallback) {
            std::cerr << "[ERROR][WaveManager] createEntityCallback not set!\n";
            continue;
        }

        // crée l'entité via ServerGame pour avoir tous les composants
        createEntityCallback(entityType, data);
    }
}

void WaveManager::addWave(std::vector<std::unique_ptr<EntityPersonnalization>> wave) {
    entityWaveCreationArray.push_back(std::move(wave));
}

void WaveManager::setWave(size_t wave) {
    if (wave >= entityWaveCreationArray.size())
        return;

    std::cerr << "[DEBUG][WaveManager] setWave " << wave << "\n";

    clock.restart();
    waveEntities.clear();

    for (const auto& e : entityWaveCreationArray[wave]) {
        waveEntities.push_back(
            std::make_unique<EntityPersonnalization>(
                factory,
                e->getType(),
                e->getCreationTimer()
            )
        );
    }

    std::cerr << "[DEBUG][WaveManager] waveEntities size = "
              << waveEntities.size() << "\n";
}
