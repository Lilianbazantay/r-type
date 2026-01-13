#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <utility>
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
        default:                 return "none";
    }
}

inline EntityType StringToEntityType(const std::string& s)
{
    if (s == "enemy")  return EntityType::enemy;
    if (s == "player") return EntityType::player;
    if (s == "weapon") return EntityType::weapon;
    if (s == "bullet") return EntityType::bullet;
    return EntityType::none;
}

// =====================================================
// Entity -> JSON (CONFIG FORMAT)
// =====================================================

inline void to_json(json& j, const Entity& e)
{
    // =====================
    // ENTITY
    // =====================
    j["entity"] = {
        {"name", e.name},
        {"type", EntityTypeToString(e.type)}
    };

    // =====================
    // COMPONENTS
    // =====================
    json c = json::object();

    if (!e.hp.empty())
        c["hp"] = e.hp[0].value;

    if (!e.velocity.empty())
        c["velocity"] = e.velocity[0].value;

    if (!e.position.empty())
        c["position"] = { e.position[0].x, e.position[0].y };

    if (!e.hitbox.empty()) {
        const HitboxStruct& h = e.hitbox[0];
        c["hitbox"] = {
            {"size", { h.sizeX, h.sizeY }},
            {"layer", h.layers},
            {"mask",  h.masks}
        };
    }

    if (!e.attack.empty()) {
        const AttackStruct& a = e.attack[0];
        c["attack"] = {
            {"damage", a.damage},
            {"fireRate", a.fireRate}
        };
    }

    if (!e.clock.empty())
        c["clock"] = {
            {"elapsedTime", e.clock[0].elapsedTime}
        };

    if (!e.cooldown.empty()) {
        const CooldownStruct& cd = e.cooldown[0];
        c["cooldown"] = {
            {"length", cd.length},
            {"remaining", cd.remaining},
            {"timestamp_activation", cd.timestamp_activation}
        };
    }

    if (!e.direction.empty()) {
        const DirectionStruct& d = e.direction[0];
        c["direction"] = {
            {"x", d.x},
            {"y", d.y},
            {"angle", d.angle}
        };
    }

    if (!e.entitySpawner.empty()) {
        const EntitySpawnerStruct& es = e.entitySpawner[0];
        c["entitySpawner"] = {
            {"cooldown_length", es.cooldown_length},
            {"is_activated", es.is_activated},
            {"entityIndex", es.entityIndex}
        };
    }

    if (!e.gravity.empty()) {
        const GravityStruct& g = e.gravity[0];
        c["gravity"] = {
            {"is_activated", g.is_activated},
            {"attraction_strength", g.attraction_strength}
        };
    }

    // ---------------------
    // AnimatedSprite
    // ---------------------
    if (!e.animatedSprite.empty()) {
        json arr = json::array();
        for (const auto& a : e.animatedSprite) {
            arr.push_back({
                {"size_x", a.size_x},
                {"size_y", a.size_y},
                {"animation_rate", a.animation_rate},
                {"is_visible", a.is_visible},
                {"idle",  {{"path", a.idle.path}, {"number_of_sprite_x", a.idle.number_of_sprite_x}, {"number_of_sprite_y", a.idle.number_of_sprite_y}}},
                {"up",    {{"path", a.up.path},   {"number_of_sprite_x", a.up.number_of_sprite_x},   {"number_of_sprite_y", a.up.number_of_sprite_y}}},
                {"down",  {{"path", a.down.path}, {"number_of_sprite_x", a.down.number_of_sprite_x}, {"number_of_sprite_y", a.down.number_of_sprite_y}}},
                {"shoot", {{"path", a.shoot.path},{"number_of_sprite_x", a.shoot.number_of_sprite_x},{"number_of_sprite_y", a.shoot.number_of_sprite_y}}},
                {"death", {{"path", a.death.path},{"number_of_sprite_x", a.death.number_of_sprite_x},{"number_of_sprite_y", a.death.number_of_sprite_y}}}
            });
        }
        c["animatedSprite"] = arr;
    }

    // ---------------------
    // Sprite
    // ---------------------
    if (!e.sprite.empty()) {
        json arr = json::array();
        for (const auto& s : e.sprite) {
            arr.push_back({
                {"size_x", s.size_x},
                {"size_y", s.size_y},
                {"is_visible", s.is_visible},
                {"idle",  {{"path", s.idle.path}}},
                {"up",    {{"path", s.up.path}}},
                {"down",  {{"path", s.down.path}}},
                {"shoot", {{"path", s.shoot.path}}},
                {"death", {{"path", s.death.path}}}
            });
        }
        c["sprite"] = arr;
    }

    // ---------------------
    // Sound
    // ---------------------
    if (!e.sound.empty()) {
        json arr = json::array();
        for (const auto& s : e.sound) {
            arr.push_back({
                {"volume", s.volume},
                {"is_looping", s.is_looping},
                {"idle",  {{"path", s.idle.path}}},
                {"up",    {{"path", s.up.path}}},
                {"down",  {{"path", s.down.path}}},
                {"shoot", {{"path", s.shoot.path}}},
                {"death", {{"path", s.death.path}}}
            });
        }
        c["sound"] = arr;
    }

    j["components"] = c;

    // =====================
    // DATA
    // =====================
    json d = json::object();

    if (!e.strategy.empty()) {
        d["strategy"] = {
            {"patterns", e.strategy[0].pattern}
        };
    }

    j["data"] = d;
}

// =====================================================
// JSON -> Entity
// =====================================================

inline void from_json(const json& j, Entity& e)
{
    e = Entity{}; // reset propre

    // =====================
    // ENTITY
    // =====================
    const json& je = j.at("entity");
    e.name = je.at("name").get<std::string>();
    e.type = StringToEntityType(je.at("type").get<std::string>());

    // =====================
    // COMPONENTS
    // =====================
    const json& c = j.at("components");

    if (c.contains("hp"))
        e.hp.push_back({ c.at("hp").get<int>() });

    if (c.contains("velocity"))
        e.velocity.push_back({ c.at("velocity").get<float>() });

    if (c.contains("position")) {
        PositionStruct p;
        p.x = c.at("position")[0].get<float>();
        p.y = c.at("position")[1].get<float>();
        e.position.push_back(p);
    }

    if (c.contains("hitbox")) {
        HitboxStruct h;
        h.sizeX = c.at("hitbox").at("size")[0].get<float>();
        h.sizeY = c.at("hitbox").at("size")[1].get<float>();
        h.layers = c.at("hitbox").at("layer").get<std::vector<int>>();
        h.masks  = c.at("hitbox").at("mask").get<std::vector<int>>();
        e.hitbox.push_back(h);
    }

    if (c.contains("attack")) {
        AttackStruct a;
        a.damage   = c.at("attack").at("damage").get<float>();
        a.fireRate = c.at("attack").at("fireRate").get<float>();
        e.attack.push_back(a);
    }

    if (c.contains("clock"))
        e.clock.push_back({
            c.at("clock").at("elapsedTime").get<float>()
        });

    if (c.contains("cooldown")) {
        CooldownStruct cd;
        cd.length = c.at("cooldown").at("length").get<double>();
        cd.remaining = c.at("cooldown").at("remaining").get<double>();
        cd.timestamp_activation =
            c.at("cooldown").at("timestamp_activation").get<double>();
        e.cooldown.push_back(cd);
    }

    if (c.contains("direction")) {
        DirectionStruct d;
        d.x = c.at("direction").at("x").get<float>();
        d.y = c.at("direction").at("y").get<float>();
        d.angle = c.at("direction").at("angle").get<float>();
        e.direction.push_back(d);
    }

    if (c.contains("entitySpawner")) {
        EntitySpawnerStruct es;
        es.cooldown_length =
            c.at("entitySpawner").at("cooldown_length").get<double>();
        es.is_activated =
            c.at("entitySpawner").at("is_activated").get<bool>();
        es.entityIndex =
            c.at("entitySpawner").at("entityIndex").get<int>();
        e.entitySpawner.push_back(es);
    }

    if (c.contains("gravity")) {
        GravityStruct g;
        g.is_activated =
            c.at("gravity").at("is_activated").get<bool>();
        g.attraction_strength =
            c.at("gravity").at("attraction_strength").get<float>();
        e.gravity.push_back(g);
    }

    // ---------------------
    // AnimatedSprite
    // ---------------------
    if (c.contains("animatedSprite")) {
        for (const auto& a : c.at("animatedSprite")) {
            AnimatedSpriteStruct as;
            as.size_x = a.at("size_x").get<float>();
            as.size_y = a.at("size_y").get<float>();
            as.animation_rate = a.at("animation_rate").get<float>();
            as.is_visible = a.at("is_visible").get<bool>();

            auto loadCtx = [](const json& jctx) -> AnimatedSpriteContext {
                AnimatedSpriteContext ctx;
                ctx.path = jctx.at("path").get<std::string>();
                ctx.number_of_sprite_x = jctx.at("number_of_sprite_x").get<int>();
                ctx.number_of_sprite_y = jctx.at("number_of_sprite_y").get<int>();
                return ctx;
            };

            as.idle  = loadCtx(a.at("idle"));
            as.up    = loadCtx(a.at("up"));
            as.down  = loadCtx(a.at("down"));
            as.shoot = loadCtx(a.at("shoot"));
            as.death = loadCtx(a.at("death"));

            e.animatedSprite.push_back(as);
        }
    }

    // ---------------------
    // Sprite
    // ---------------------
    if (c.contains("sprite")) {
        for (const auto& s : c.at("sprite")) {
            SpriteStruct ss;
            ss.size_x = s.at("size_x").get<float>();
            ss.size_y = s.at("size_y").get<float>();
            ss.is_visible = s.at("is_visible").get<bool>();

            auto loadCtx = [](const json& jctx) -> SpriteContext {
                SpriteContext ctx;
                ctx.path = jctx.at("path").get<std::string>();
                return ctx;
            };

            ss.idle  = loadCtx(s.at("idle"));
            ss.up    = loadCtx(s.at("up"));
            ss.down  = loadCtx(s.at("down"));
            ss.shoot = loadCtx(s.at("shoot"));
            ss.death = loadCtx(s.at("death"));

            e.sprite.push_back(ss);
        }
    }

    // ---------------------
    // Sound
    // ---------------------
    if (c.contains("sound")) {
        for (const auto& s : c.at("sound")) {
            SoundStruct ss;
            ss.volume = s.at("volume").get<float>();
            ss.is_looping = s.at("is_looping").get<bool>();

            auto loadCtx = [](const json& jctx) -> SoundContext {
                SoundContext ctx;
                ctx.path = jctx.at("path").get<std::string>();
                return ctx;
            };

            ss.idle  = loadCtx(s.at("idle"));
            ss.up    = loadCtx(s.at("up"));
            ss.down  = loadCtx(s.at("down"));
            ss.shoot = loadCtx(s.at("shoot"));
            ss.death = loadCtx(s.at("death"));

            e.sound.push_back(ss);
        }
    }

    // =====================
    // DATA
    // =====================
    if (j.contains("data")) {
        const json& d = j.at("data");

        if (d.contains("strategy")) {
            StrategyStruct st;
            st.pattern =
                d.at("strategy").at("patterns")
                    .get<std::vector<std::pair<int,int>>>();
            e.strategy.push_back(st);
        }
    }
}
