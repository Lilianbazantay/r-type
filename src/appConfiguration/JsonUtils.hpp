#pragma once

#include <nlohmann/json.hpp>
#include "Entity.hpp"
using json = nlohmann::json;

// =====================
// Component JSON Serialization
// =====================

// AnimatedSpriteStruct
inline void to_json(json& j, const AnimatedSpriteStruct& a) {
    j = json{
        {"file_path", a.file_path},
        {"size_x", a.size_x},
        {"size_y", a.size_y},
        {"number_of_sprite_x", a.number_of_sprite_x},
        {"number_of_sprite_y", a.number_of_sprite_y},
        {"animation_rate", a.animation_rate},
        {"is_visible", a.is_visible}
    };
}
inline void from_json(const json& j, AnimatedSpriteStruct& a) {
    j.at("file_path").get_to(a.file_path);
    j.at("size_x").get_to(a.size_x);
    j.at("size_y").get_to(a.size_y);
    j.at("number_of_sprite_x").get_to(a.number_of_sprite_x);
    j.at("number_of_sprite_y").get_to(a.number_of_sprite_y);
    j.at("animation_rate").get_to(a.animation_rate);
    j.at("is_visible").get_to(a.is_visible);
}

// AttackStruct
inline void to_json(json& j, const AttackStruct& a) {
    j = json{{"damage", a.damage}, {"fireRate", a.fireRate}, {"remaining", a.remaining}};
}
inline void from_json(const json& j, AttackStruct& a) {
    j.at("damage").get_to(a.damage);
    j.at("fireRate").get_to(a.fireRate);
    j.at("remaining").get_to(a.remaining);
}

// ClockStruct
inline void to_json(json& j, const ClockStruct& c) {
    j = json{{"elapsedTime", c.elapsedTime}};
}
inline void from_json(const json& j, ClockStruct& c) {
    j.at("elapsedTime").get_to(c.elapsedTime);
}

// CooldownStruct
inline void to_json(json& j, const CooldownStruct& c) {
    j = json{{"length", c.length}, {"remaining", c.remaining}, {"timestamp_activation", c.timestamp_activation}};
}
inline void from_json(const json& j, CooldownStruct& c) {
    j.at("length").get_to(c.length);
    j.at("remaining").get_to(c.remaining);
    j.at("timestamp_activation").get_to(c.timestamp_activation);
}

// DirectionStruct
inline void to_json(json& j, const DirectionStruct& d) {
    j = json{{"x", d.x}, {"y", d.y}, {"angle", d.angle}};
}
inline void from_json(const json& j, DirectionStruct& d) {
    j.at("x").get_to(d.x);
    j.at("y").get_to(d.y);
    j.at("angle").get_to(d.angle);
}

// EntitySpawnerStruct
inline void to_json(json& j, const EntitySpawnerStruct& e) {
    j = json{{"cooldown_length", e.cooldown_length}, {"is_activated", e.is_activated}, {"entityIndex", e.entityIndex}};
}
inline void from_json(const json& j, EntitySpawnerStruct& e) {
    j.at("cooldown_length").get_to(e.cooldown_length);
    j.at("is_activated").get_to(e.is_activated);
    j.at("entityIndex").get_to(e.entityIndex);
}

// GravityStruct
inline void to_json(json& j, const GravityStruct& g) {
    j = json{{"is_activated", g.is_activated}, {"attraction_strength", g.attraction_strength}};
}
inline void from_json(const json& j, GravityStruct& g) {
    j.at("is_activated").get_to(g.is_activated);
    j.at("attraction_strength").get_to(g.attraction_strength);
}

// HitboxStruct
inline void to_json(json& j, const HitboxStruct& h) {
    j = json{{"sizeX", h.sizeX}, {"sizeY", h.sizeY}, {"damage", h.damage}, {"masks", h.masks}, {"layers", h.layers}};
}
inline void from_json(const json& j, HitboxStruct& h) {
    j.at("sizeX").get_to(h.sizeX);
    j.at("sizeY").get_to(h.sizeY);
    j.at("damage").get_to(h.damage);
    j.at("masks").get_to(h.masks);
    j.at("layers").get_to(h.layers);
}

// HpStruct
inline void to_json(json& j, const HpStruct& h) { j = json{{"value", h.value}}; }
inline void from_json(const json& j, HpStruct& h) { j.at("value").get_to(h.value); }

// PositionStruct
inline void to_json(json& j, const PositionStruct& p) { j = json{{"x", p.x}, {"y", p.y}}; }
inline void from_json(const json& j, PositionStruct& p) { j.at("x").get_to(p.x); j.at("y").get_to(p.y); }

// SoundStruct
inline void to_json(json& j, const SoundStruct& s) { j = json{{"file_path", s.file_path}, {"volume", s.volume}, {"is_looping", s.is_looping}}; }
inline void from_json(const json& j, SoundStruct& s) { j.at("file_path").get_to(s.file_path); j.at("volume").get_to(s.volume); j.at("is_looping").get_to(s.is_looping); }

// SpriteStruct
inline void to_json(json& j, const SpriteStruct& s) { j = json{{"file_path", s.file_path}, {"size_x", s.size_x}, {"size_y", s.size_y}, {"is_visible", s.is_visible}}; }
inline void from_json(const json& j, SpriteStruct& s) { j.at("file_path").get_to(s.file_path); j.at("size_x").get_to(s.size_x); j.at("size_y").get_to(s.size_y); j.at("is_visible").get_to(s.is_visible); }

// StrategyStruct
inline void to_json(json& j, const StrategyStruct& s) { j = json{{"pattern", s.pattern}}; }
inline void from_json(const json& j, StrategyStruct& s) { j.at("pattern").get_to(s.pattern); }

// VelocityStruct
inline void to_json(json& j, const VelocityStruct& v) { j = json{{"value", v.value}}; }
inline void from_json(const json& j, VelocityStruct& v) { j.at("value").get_to(v.value); }

// =====================
// Entity JSON Serialization (global namespace)
// =====================
inline void to_json(json& j, const Entity& e) {
    j = json{
        {"name", e.name},
        {"type", static_cast<int>(e.type)},
        {"animatedSprite", e.animatedSprite},
        {"attack", e.attack},
        {"clock", e.clock},
        {"cooldown", e.cooldown},
        {"direction", e.direction},
        {"entitySpawner", e.entitySpawner},
        {"gravity", e.gravity},
        {"hitbox", e.hitbox},
        {"hp", e.hp},
        {"position", e.position},
        {"sound", e.sound},
        {"sprite", e.sprite},
        {"strategy", e.strategy},
        {"velocity", e.velocity}
    };
}

inline void from_json(const json& j, Entity& e) {
    int t;
    j.at("name").get_to(e.name);
    j.at("type").get_to(t);
    e.type = static_cast<EntityType>(t);

    j.at("animatedSprite").get_to(e.animatedSprite);
    j.at("attack").get_to(e.attack);
    j.at("clock").get_to(e.clock);
    j.at("cooldown").get_to(e.cooldown);
    j.at("direction").get_to(e.direction);
    j.at("entitySpawner").get_to(e.entitySpawner);
    j.at("gravity").get_to(e.gravity);
    j.at("hitbox").get_to(e.hitbox);
    j.at("hp").get_to(e.hp);
    j.at("position").get_to(e.position);
    j.at("sound").get_to(e.sound);
    j.at("sprite").get_to(e.sprite);
    j.at("strategy").get_to(e.strategy);
    j.at("velocity").get_to(e.velocity);
}
