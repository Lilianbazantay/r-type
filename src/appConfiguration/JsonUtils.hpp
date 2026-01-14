#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include "Entity.hpp"

using json = nlohmann::json;

// =====================================================
// EntityType <-> string
// =====================================================

inline std::string EntityTypeToString(EntityType type)
{
    switch (type)
    {
        case EntityType::enemy:  return "enemy";
        case EntityType::player: return "player";
        case EntityType::weapon: return "weapon";
        case EntityType::bullet: return "bullet";
        case EntityType::map:    return "map";
        default:                 return "none";
    }
}

inline EntityType StringToEntityType(const std::string& s)
{
    if (s == "enemy")  return EntityType::enemy;
    if (s == "player") return EntityType::player;
    if (s == "weapon") return EntityType::weapon;
    if (s == "bullet") return EntityType::bullet;
    if (s == "map")    return EntityType::map;
    return EntityType::none;
}

// =====================================================
// Entity -> JSON
// =====================================================

inline void to_json(json& j, const Entity& e)
{
    j["entity"] = {
        {"name", e.name},
        {"type", EntityTypeToString(e.type)}
    };

    json c = json::object();

    if (!e.position.empty())
        c["position"] = { e.position[0].pos.x, e.position[0].pos.y };

    if (!e.velocity.empty())
        c["velocity"] = e.velocity[0].value;

    if (!e.direction.empty())
        c["direction"] = { e.direction[0].vec.x, e.direction[0].vec.y };

    if (!e.hp.empty())
        c["hp"] = e.hp[0].value;

    if (!e.hitbox.empty()) {
        const HitboxStruct& h = e.hitbox[0];
        c["hitbox"] = {
            {"size", { h.size.x, h.size.y }},
            {"layer", h.layers},
            {"mask", h.masks},
            {"damage", h.damage}
        };
    }

    if (!e.animatedSprite.empty()) {
        const AnimatedSpriteStruct& a = e.animatedSprite[0];
        json animations = json::object();
        auto addAnim = [&](const std::string& name, const AnimatedSpriteContext& ctx){
            animations[name] = {
                {"frame_size", {ctx.size.x, ctx.size.y}},
                {"start", {ctx.start.x, ctx.start.y}},
                {"step", {ctx.step.x, ctx.step.y}},
                {"length", ctx.lenth},
                {"loop", ctx.loop}
            };
        };
        addAnim("idle", a.idle);
        addAnim("walk", a.up);
        addAnim("death", a.death);

        c["animatedsprite"] = {
            {"spritesheet", a.idle.path},
            {"frame_rate", a.animation_rate},
            {"default", "idle"},
            {"animations", animations}
        };
    }

    if (!e.sprite.empty()) {
        json jSprites = json::array();
        for (const auto& s : e.sprite) {
            jSprites.push_back({
                {"path", s.path},
                {"size_x", s.size_x},
                {"size_y", s.size_y},
                {"is_visible", s.is_visible}
            });
        }
        c["sprite"] = jSprites;
    }

    if (!e.clock.empty())
        c["clock"] = e.clock[0].is_started;

    if (!e.cooldown.empty())
        c["cooldown"] = e.cooldown[0].length;

    if (!e.gravity.empty())
        c["gravity"] = { {"strength", e.gravity[0].strength} };

    if (!e.sound.empty()) {
        const SoundStruct& s = e.sound[0];
        json sounds = json::object();
        auto addSound = [&](const std::string& name, const SoundContext& ctx){
            sounds[name] = {
                {"file_path", ctx.path},
                {"volume", ctx.volume},
                {"loop", ctx.loop}
            };
        };
        addSound("idle", s.idle);
        addSound("shoot", s.shoot);
        addSound("hit", s.hit);
        addSound("death", s.death);

        c["sound"] = {
            {"default", "idle"},
            {"sounds", sounds}
        };
    }

    if (!e.strategy.empty()) {
        json patterns = json::array();
        for (const auto& pat : e.strategy[0].strategy) {
            json sequences = json::array();
            for (const auto& seq : pat.movements)
                sequences.push_back({ { seq.dictio.first, seq.dictio.second }, seq.value });
            patterns.push_back({
                {"hp_range", { pat.hp_range.x, pat.hp_range.y }},
                {"sequences", sequences}
            });
        }
        c["strategy"] = { {"patterns", patterns} };
    }

    if (!e.entitySpawner.empty()) {
        const EntitySpawnerStruct& es = e.entitySpawner[0];
        c["entityspawner"] = {
            {"entity_type", "basic_bullet"},
            {"position", { es.position.x, es.position.y }},
            {"cooldown", es.cooldown_length},
            {"is_activated", es.is_activated}
        };
    }

    j["components"] = c;
}

// =====================================================
// JSON -> Entity
// =====================================================

inline void from_json(const json& j, Entity& e)
{
    e = Entity{}; // reset

    const json& je = j.at("entity");
    e.name = je.at("name").get<std::string>();
    e.type = StringToEntityType(je.at("type").get<std::string>());

    const json& c = j.at("components");

    if (c.contains("position")) {
        PositionStruct p;
        p.pos.x = c.at("position")[0].get<float>();
        p.pos.y = c.at("position")[1].get<float>();
        e.position.push_back(p);
    }

    if (c.contains("velocity"))
        e.velocity.push_back({ c.at("velocity").get<float>() });

    if (c.contains("direction")) {
        DirectionStruct d;
        d.vec.x = c.at("direction")[0].get<float>();
        d.vec.y = c.at("direction")[1].get<float>();
        e.direction.push_back(d);
    }

    if (c.contains("hp"))
        e.hp.push_back({ c.at("hp").get<int>() });

    if (c.contains("hitbox")) {
        HitboxStruct h;
        h.size.x = c.at("hitbox").at("size")[0].get<int>();
        h.size.y = c.at("hitbox").at("size")[1].get<int>();
        h.layers = c.at("hitbox").at("layer").get<std::vector<int>>();
        h.masks  = c.at("hitbox").at("mask").get<std::vector<int>>();
        h.damage = c.at("hitbox").at("damage").get<int>();
        e.hitbox.push_back(h);
    }

    if (c.contains("animatedsprite")) {
        AnimatedSpriteStruct a;
        const auto& anim = c.at("animatedsprite");
        a.idle.path = anim.at("spritesheet").get<std::string>();
        a.animation_rate = anim.at("frame_rate").get<float>();
        a.idle.size = sf::Vector2f(anim.at("animations").at("idle").at("frame_size")[0],
                                   anim.at("animations").at("idle").at("frame_size")[1]);
        a.idle.start = sf::Vector2i(anim.at("animations").at("idle").at("start")[0],
                                     anim.at("animations").at("idle").at("start")[1]);
        a.idle.step = sf::Vector2i(anim.at("animations").at("idle").at("step")[0],
                                    anim.at("animations").at("idle").at("step")[1]);
        a.idle.lenth = anim.at("animations").at("idle").at("length").get<int>();
        a.idle.loop = anim.at("animations").at("idle").value("loop", true);

        e.animatedSprite.push_back(a);
    }

    if (c.contains("sprite")) {
        for (const auto& s : c.at("sprite")) {
            SpriteStruct sp;
            sp.path = s.at("path").get<std::string>();
            sp.size_x = s.at("size_x").get<float>();
            sp.size_y = s.at("size_y").get<float>();
            sp.is_visible = s.value("is_visible", true);
            e.sprite.push_back(sp);
        }
    }

    if (c.contains("clock"))
        e.clock.push_back({ c.at("clock").get<bool>() });

    if (c.contains("cooldown"))
        e.cooldown.push_back({ c.at("cooldown").get<double>() });

    if (c.contains("gravity")) {
        GravityStruct g;
        g.strength = c.at("gravity").at("strength").get<float>();
        e.gravity.push_back(g);
    }

    if (c.contains("sound")) {
        SoundStruct s;
        const auto& sounds = c.at("sound").at("sounds");
        auto loadSound = [&](SoundContext& ctx, const std::string& name){
            ctx.path = sounds.at(name).at("file_path").get<std::string>();
            ctx.volume = sounds.at(name).at("volume").get<float>();
            ctx.loop = sounds.at(name).at("loop").get<bool>();
        };
        loadSound(s.idle, "idle");
        loadSound(s.shoot, "shoot");
        loadSound(s.hit, "hit");
        loadSound(s.death, "death");
        e.sound.push_back(s);
    }

    if (c.contains("strategy")) {
        StrategyStruct st;
        for (const auto& pat : c.at("strategy").at("patterns")) {
            Pattern p;
            p.hp_range.x = pat.at("hp_range")[0].get<int>();
            p.hp_range.y = pat.at("hp_range")[1].get<int>();
            for (const auto& seq : pat.at("sequences")) {
                Move m;
                m.dictio.first = seq[0][0].get<int>();
                m.dictio.second = seq[0][1].get<int>();
                m.value = seq[1].get<int>();
                p.movements.push_back(m);
            }
            st.strategy.push_back(p);
        }
        e.strategy.push_back(st);
    }

    if (c.contains("entityspawner")) {
        EntitySpawnerStruct es;
        es.position.x = c.at("entityspawner").at("position")[0].get<float>();
        es.position.y = c.at("entityspawner").at("position")[1].get<float>();
        es.cooldown_length = c.at("entityspawner").at("cooldown").get<double>();
        es.is_activated = c.at("entityspawner").at("is_activated").get<bool>();
        e.entitySpawner.push_back(es);
    }
}
