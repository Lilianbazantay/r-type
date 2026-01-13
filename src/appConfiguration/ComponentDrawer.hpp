#pragma once

#include <vector>

#include "Entity.hpp"

class ComponentDrawer
{
private:

public:
    ~ComponentDrawer() = default;

    static void drawComponentList(std::vector<AnimatedSpriteStruct> &vec, const char* label, const char*);
    static void drawComponentList(std::vector<ClockStruct> &vec, const char* label, const char*);
    static void drawComponentList(std::vector<CooldownStruct> &vec, const char* label, const char*);
    static void drawComponentList(std::vector<DirectionStruct> &vec, const char* label, const char*);
    static void drawComponentList(std::vector<EntitySpawnerStruct> &vec, const std::vector<Entity>& allEntities, const char* label, const char*);
    static void drawComponentList(std::vector<GravityStruct> &vec, const char* label, const char*);
    static void drawComponentList(std::vector<HitboxStruct> &vec, const char* label, const char*);
    static void drawComponentList(std::vector<HpStruct> &vec, const char* label, const char*);
    static void drawComponentList(std::vector<PositionStruct> &vec, const char* label, const char*);
    static void drawComponentList(std::vector<SoundStruct> &vec, const char* label, const char*);
    static void drawComponentList(std::vector<SpriteStruct> &vec, const char* label, const char*);
    static void drawComponentList(std::vector<StrategyStruct> &vec, const char* label, const char*);
    static void drawComponentList(std::vector<VelocityStruct> &vec, const char* label, const char*);
};
