#pragma once
#include <vector>
#include "Entity.hpp"

/**
 * @brief This class contain every function that Display a component with polymorphism
 */
class ComponentDrawer
{
public:
    static void drawComponentList(std::vector<AnimatedSpriteStruct>&, const char*, const char*);
    static void drawComponentList(std::vector<ClockStruct>&, const char*, const char*);
    static void drawComponentList(std::vector<CooldownStruct>&, const char*, const char*);
    static void drawComponentList(std::vector<DirectionStruct>&, const char*, const char*);
    static void drawComponentList(std::vector<EntitySpawnerStruct>&, const std::vector<Entity>&, const char*, const char*);
    static void drawComponentList(std::vector<GravityStruct>&, const char*, const char*);
    static void drawComponentList(std::vector<HitboxStruct>&, const char*, const char*);
    static void drawComponentList(std::vector<HpStruct>&, const char*, const char*);
    static void drawComponentList(std::vector<PositionStruct>&, const char*, const char*);
    static void drawComponentList(std::vector<SoundStruct>&, const char*, const char*);
    static void drawComponentList(std::vector<SpriteStruct>&, const char*, const char*);
    static void drawComponentList(std::vector<StrategyStruct>&, const char*, const char*);
    static void drawComponentList(std::vector<VelocityStruct>&, const char*, const char*);
    static void drawPathSelector(const char* label, std::string& path, const std::vector<std::string>& extensions, const std::string& startDir = "assets");
};
