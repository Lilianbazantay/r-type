
#pragma once

#include "ecs/Component/Hitbox.hpp"
#include "ecs/Entity/IMediatorEntity.hpp"
#include "ecs/relevant_data.hpp"
#include <SFML/System/Clock.hpp>
#include <utility>

class EntityPersonnalization {
    private:
        int entityType = ENTITY_ENEMY;
        std::pair<float, float> pos = {};
        std::string spritePath = "./assets/Enemy.png";
        std::vector<std::pair<std::pair<float, float>, float>> strategy = {};
        int damage = 1;
        std::vector<int> layers = {2};
        std::vector<int> masks = {1};
        std::pair<size_t, size_t> size = {64, 64};
        float creationTimer = 0;
        int hp = 0;
        int velocity = 110;
    public:
        EntityPersonnalization() = default;
        EntityPersonnalization(std::pair<float, float>, std::vector<std::pair<std::pair<float, float>, float>>, float);
        ~EntityPersonnalization() = default;

        void setEntityPos(std::pair<float, float>);
        void setHitBox(int, std::vector<int>, std::vector<int>);
        void setSize(size_t, size_t);
        void setSize(std::pair<size_t, size_t>);
        void setSprite(std::string);
        void setType(int);
        void setVelocity(int);
        void setHp(int);
        void setCreationTimer(float);
        void setStrategy(std::vector<std::pair<std::pair<float, float>, float>>);
        void pushStrategy(std::pair<std::pair<float, float>, float>);

        float getCreationTimer();
        int getType();
        std::unique_ptr<IMediatorEntity> getEntity();
};

class WaveManager {
    private:
        sf::Clock clock;
        std::vector<EntityPersonnalization> waveEntities = {};
        std::vector<std::vector<EntityPersonnalization>> entityWaveCreationArray = {};
    public:
        WaveManager();
        ~WaveManager() = default;
        void computeEntities(relevant_data_t*);
        void setWave(size_t);
        void addWave(std::vector<EntityPersonnalization>);
};
