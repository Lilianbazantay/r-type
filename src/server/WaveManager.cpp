
#include "WaveManager.hpp"
#include "ecs/Component/Hitbox.hpp"
#include "ecs/Component/Sprite.hpp"
#include "ecs/Entity/Entities.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/relevant_data.hpp"
#include <memory>


EntityPersonnalization::EntityPersonnalization(std::pair<float, float> _pos, std::vector<std::pair<std::pair<float, float>, float>> _strat, float _timer) {
    pos = _pos;
    strategy = _strat;
    creationTimer = _timer;
}

void EntityPersonnalization::setEntityPos(std::pair<float, float> _newPos) {
    pos = _newPos;
}

void EntityPersonnalization::setHitBox(int _newDamage, std::vector<int> _newLayer, std::vector<int> _newmask) {
    damage = _newDamage;
    layers = _newLayer;
    masks = _newmask;
}

void EntityPersonnalization::setSize(size_t _x, size_t _y) {
    size = {_x, _y};
}

void EntityPersonnalization::setSize(std::pair<size_t, size_t> _newSize) {
    size = _newSize;
}

void EntityPersonnalization::setSprite(std::string _newPath) {
    spritePath = _newPath;
}

void EntityPersonnalization::setType(int _newType) {
    entityType = _newType;
}

void EntityPersonnalization::setVelocity(int _newVelocity) {
    velocity = _newVelocity;
}

void EntityPersonnalization::setHp(int _newHp) {
    hp = _newHp;
}

void EntityPersonnalization::setCreationTimer(float _newTimer) {
    creationTimer = _newTimer;
}

void EntityPersonnalization::setStrategy(std::vector<std::pair<std::pair<float, float>, float>> _newStrategy) {
    strategy = _newStrategy;
}

void EntityPersonnalization::pushStrategy(std::pair<std::pair<float, float>, float> _Pushed) {
    strategy.push_back(_Pushed);
}

float EntityPersonnalization::getCreationTimer() {
    return creationTimer;
}

int EntityPersonnalization::getType() {
    return entityType;
}

std::unique_ptr<IMediatorEntity> EntityPersonnalization::getEntity() {
    Sprite _spr(spritePath, size.first, size.second);
    Hitbox _hitbox(size.first, size.second, damage, layers, masks);
    Position _pos(pos.first, pos.second);
    Strategy _strat(strategy);
    switch (entityType) {
        case (ENTITY_BULLET): {
            return std::make_unique<EnemyBullet>();
            break;
        }
        case (ENTITY_ENEMY): {
            return std::make_unique<Enemy>(_spr, _hitbox, _pos, _strat, velocity, hp);
            break;
        }
        default: {
            return nullptr;
            break;
        }
    }
}

WaveManager::WaveManager() {
    std::vector<std::pair<std::pair<float, float>, float>> strat0 = {{{-1, 1}, 3}, {{2, 0.5}, 0}, {{0, 1.5}, 1}, {{2, 0.5}, 0}, {{0, 1.5}, 1}, {{2, 0.5}, 0}, {{1, 1}, 3}, {{0, -2}, 4.5}, {{-2, -2}, 0}};
    std::vector<EntityPersonnalization> w0 = {};
    EntityPersonnalization wall ({2120, 0}, {{{-2, -2}, 0}}, 0);
    wall.setSprite("./assets/empty.jpg");
    wall.setType(ENTITY_ENEMY);
    wall.setSize(0, 1300);
    wall.setHitBox(40, {1, 2, 3, 4, 5}, std::vector<int> {1, 2, 3, 4, 5});
    wall.setVelocity(0);
    wall.setHp(2147483647);
    w0.push_back(wall);
    wall.setEntityPos({0, -100});
    w0.push_back(wall);
    wall.setSize(2000, 1);
    w0.push_back(wall);
    wall.setEntityPos({0, 1680});
    w0.push_back(wall);
    for (size_t i = 0; i < 5; i++) {
        w0.push_back(EntityPersonnalization({1920, 0}, strat0, i * 3));
    }
    addWave(w0);
    setWave(0);
}

void WaveManager::computeEntities(relevant_data_t *data) {
    int newId = 0;
    if (waveEntities.empty())
        return;
    if (waveEntities[0].getCreationTimer() > clock.getElapsedTime().asSeconds())
        return;
    for (; waveEntities[0].getCreationTimer() <= clock.getElapsedTime().asSeconds() && !waveEntities.empty();
        waveEntities.erase(waveEntities.begin())) {
            switch (waveEntities[0].getType()) {
                case (ENTITY_ENEMY):
                    newId = data->enemy_count;
                    data->enemy_count++;
                    break;
                case (ENTITY_BULLET):
                    newId = data->bullet_count;
                    data->bullet_count++;
                    break;
                default:
                    break;
            }
            data->entityList.push_back(waveEntities[0].getEntity());
            data->entityList[data->entityList.size() -1]->setId(newId);
        }
}

void WaveManager::addWave(std::vector<EntityPersonnalization> _newWave) {
    entityWaveCreationArray.push_back(_newWave);
}

void WaveManager::setWave(size_t _wave) {
    if (_wave >= entityWaveCreationArray.size())
        return;
    clock.restart();
    waveEntities = entityWaveCreationArray[_wave];
}
