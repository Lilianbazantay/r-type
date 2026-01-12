#pragma once

#include <string>
#include <vector>
#include <utility>

enum class EntityType {
    enemy,
    player,
    weapon,
    bullet,
    none,
};

// =====================
// Component Structures
// =====================
struct AnimatedSpriteStruct {
    std::string file_path;
    float size_x = 0.f;
    float size_y = 0.f;
    int number_of_sprite_x = 0;
    int number_of_sprite_y = 0;
    float animation_rate = 0.f;
    bool is_visible = true;
};

struct AttackStruct {
    float damage = 0.f;
    float fireRate = 0.f;
    float remaining = 0.f;
};

struct ClockStruct {
    float elapsedTime = 0.f;
};

struct CooldownStruct {
    double length = 0.0;
    double remaining = 0.0;
    double timestamp_activation = 0.0;
};

struct DirectionStruct {
    float x = 0.f;
    float y = 0.f;
    float angle = 0.f;
};

struct EntitySpawnerStruct {
    double cooldown_length = 0.0;
    bool is_activated = false;
    int entityIndex = -1;
};

struct GravityStruct {
    bool is_activated = false;
    float attraction_strength = 0.f;
};

struct HitboxStruct {
    float sizeX = 0.f;
    float sizeY = 0.f;
    int damage = 0;
    std::vector<int> masks;
    std::vector<int> layers;
};

struct HpStruct {
    int value = 0;
};

struct PositionStruct {
    float x = 0.f;
    float y = 0.f;
};

struct SoundStruct {
    std::string file_path;
    float volume = 100.f;
    bool is_looping = false;
};

struct SpriteStruct {
    std::string file_path;
    float size_x = 0.f;
    float size_y = 0.f;
    bool is_visible = true;
};

struct StrategyStruct {
    std::vector<std::pair<int,int>> pattern;
};

struct VelocityStruct {
    float value = 0.f;
};

// =====================
// Entity Class
// =====================
class Entity {
public:
    std::string name = "new entity";
    EntityType type = EntityType::none;

    std::vector<AnimatedSpriteStruct> animatedSprite;
    std::vector<AttackStruct> attack;
    std::vector<ClockStruct> clock;
    std::vector<CooldownStruct> cooldown;
    std::vector<DirectionStruct> direction;
    std::vector<EntitySpawnerStruct> entitySpawner;
    std::vector<GravityStruct> gravity;
    std::vector<HitboxStruct> hitbox;
    std::vector<HpStruct> hp;
    std::vector<PositionStruct> position;
    std::vector<SoundStruct> sound;
    std::vector<SpriteStruct> sprite;
    std::vector<StrategyStruct> strategy;
    std::vector<VelocityStruct> velocity;

    ~Entity() = default;
};
