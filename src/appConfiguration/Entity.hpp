#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <SFML/System.hpp>

enum class EntityType {
    enemy,
    player,
    weapon,
    bullet,
    map,
    none,
};

struct AnimatedSpriteContext
{
    sf::Vector2i start;
    sf::Vector2i step;
    sf::Vector2f size;
    bool loop;
    int lenth;
};

struct AnimatedSpriteStruct
{
    AnimatedSpriteContext idle;
    AnimatedSpriteContext up;
    AnimatedSpriteContext down;
    AnimatedSpriteContext shoot;
    AnimatedSpriteContext hit;
    AnimatedSpriteContext death;

    std::string path;
    float animation_rate = 0.f;
    bool is_visible = true;
};

struct ClockStruct {
    bool is_started = false;
};

struct CooldownStruct {
    double length = 0.0;
};

struct DirectionStruct {
    sf::Vector2f vec;
};

struct EntitySpawnerStruct {
    sf::Vector2f position;
    double cooldown_length = 0.0;
    bool is_activated = false;
    int entityIndex = -1;
};

struct GravityStruct {
    bool is_activated = false;
    float strength = 0.f;
};

struct HitboxStruct {
    sf::Vector2i size;
    int damage = 0;
    std::vector<int> masks;
    std::vector<int> layers;
};

struct HpStruct { int value = 0; };
struct PositionStruct { sf::Vector2f pos; };

struct SoundContext {
    std::string path;
    float volume;
    bool loop;
};

struct SoundStruct {
    SoundContext idle;
    SoundContext up;
    SoundContext down;
    SoundContext shoot;
    SoundContext hit;
    SoundContext death;
};

struct SpriteStruct {
    std::string path;
    float size_x = 0.f;
    float size_y = 0.f;
    bool is_visible = true;
};

inline const std::unordered_map<std::string, std::pair<int,int>> StrategyDictionary = {
    {"UP",{0,1}}, {"DOWN",{0,-1}}, {"RIGHT",{1,0}}, {"LEFT",{-1,0}},
    {"UP_LEFT",{-1,1}}, {"DOWN_LEFT",{-1,-1}}, {"UP_RIGHT",{1,1}}, {"DOWN_RIGHT",{1,-1}},
    {"SHOOT",{2,2}}, {"LOOP",{-2,-2}}
};

struct Move {
    std::pair<int,int> direction;
    float value;
};

struct Pattern {
    std::vector<Move> movements;
    sf::Vector2i hp_range;
};

struct StrategyStruct {
    std::vector<Pattern> strategy;
};

struct VelocityStruct {
    float value = 0.f;
};

struct ParallaxLayer {
    std::string path;
    float speed;
};

class Entity {
public:
    std::string name = "new entity";
    EntityType type = EntityType::none;
    std::vector<ParallaxLayer> _paralaxe;

    std::vector<AnimatedSpriteStruct> animatedSprite;
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
};
