#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>

#include <vector>
#include <string>
#include <utility>

#include "Entity.hpp"

// =====================
// Utils
// =====================
const char* EntityTypeToString(EntityType type) {
    switch (type) {
        case EntityType::enemy:  return "Enemy";
        case EntityType::player: return "Player";
        case EntityType::weapon: return "Weapon";
        case EntityType::bullet: return "Bullet";
        default:                 return "None";
    }
}

// Minimal component names for the Inspector dropdown
static const char* componentNames[] = {
    "AnimatedSprite","Attack","Clock","Cooldown",
    "Direction","EntitySpawner","Gravity","Hitbox",
    "Hp","Position","Sound","Sprite",
    "Strategy","Velocity"
};

// =====================
// Draw Components
// =====================
void drawComponentList(std::vector<AnimatedSpriteStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputText("File Path", &vec[i].file_path);
            ImGui::InputFloat("Size X", &vec[i].size_x);
            ImGui::InputFloat("Size Y", &vec[i].size_y);
            ImGui::InputInt("Sprite X Count", &vec[i].number_of_sprite_x);
            ImGui::InputInt("Sprite Y Count", &vec[i].number_of_sprite_y);
            ImGui::InputFloat("Animation Rate", &vec[i].animation_rate);
            ImGui::Checkbox("Visible", &vec[i].is_visible);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<AttackStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputFloat("Damage", &vec[i].damage);
            ImGui::InputFloat("Fire Rate", &vec[i].fireRate);
            ImGui::InputFloat("Remaining", &vec[i].remaining);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<ClockStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::InputFloat(label, &vec[i].elapsedTime);
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<CooldownStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputDouble("Length", &vec[i].length);
            ImGui::InputDouble("Remaining", &vec[i].remaining);
            ImGui::InputDouble("Timestamp", &vec[i].timestamp_activation);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<DirectionStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::InputFloat("X", &vec[i].x);
        ImGui::InputFloat("Y", &vec[i].y);
        ImGui::InputFloat("Angle", &vec[i].angle);
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<EntitySpawnerStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::Checkbox("Activated", &vec[i].is_activated);
        ImGui::InputDouble("Cooldown Length", &vec[i].cooldown_length);
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<GravityStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::Checkbox("Activated", &vec[i].is_activated);
        ImGui::InputFloat("Attraction Strength", &vec[i].attraction_strength);
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<HitboxStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::InputFloat("Size X", &vec[i].sizeX);
        ImGui::InputFloat("Size Y", &vec[i].sizeY);
        ImGui::InputInt("Damage", &vec[i].damage);
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<HpStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::InputInt("Value", &vec[i].value);
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<PositionStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::InputFloat("X", &vec[i].x);
        ImGui::InputFloat("Y", &vec[i].y);
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<SoundStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::InputText("File Path", &vec[i].file_path);
        ImGui::InputFloat("Volume", &vec[i].volume);
        ImGui::Checkbox("Looping", &vec[i].is_looping);
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<SpriteStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::InputText("File Path", &vec[i].file_path);
        ImGui::InputFloat("Size X", &vec[i].size_x);
        ImGui::InputFloat("Size Y", &vec[i].size_y);
        ImGui::Checkbox("Visible", &vec[i].is_visible);
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<StrategyStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::Text("Pattern size: %zu", vec[i].pattern.size());
        ImGui::PopID();
    }
}

void drawComponentList(std::vector<VelocityStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));
        ImGui::InputFloat("Value", &vec[i].value);
        ImGui::PopID();
    }
}

// =====================
// Main
// =====================
int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Entity Editor");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    std::vector<Entity> entities;
    int selectedEntityIndex = -1;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImVec2 winSize = ImGui::GetIO().DisplaySize;
        float sideWidth = winSize.x * 0.2f;
        float bottomHeight = winSize.y * 0.2f;
        float centerWidth = winSize.x - sideWidth * 2;
        float centerHeight = winSize.y - bottomHeight;

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

        // LEFT PANEL
        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize({sideWidth, winSize.y});
        ImGui::Begin("Entities", nullptr, flags);
        ImGui::Columns(2, nullptr, false);

        for (int i = 0; i < (int)entities.size(); i++) {
            ImGui::PushID(i);
            if (ImGui::Selectable(entities[i].name.c_str(), selectedEntityIndex == i))
                selectedEntityIndex = i;
            ImGui::NextColumn();
            if (ImGui::Button("X")) {
                entities.erase(entities.begin() + i);
                selectedEntityIndex = -1;
                ImGui::PopID();
                break;
            }
            ImGui::NextColumn();
            ImGui::PopID();
        }

        ImGui::Separator();
        if (ImGui::Button("Add Entity"))
            entities.push_back({});
        ImGui::Columns(1);
        ImGui::End();

        // BOTTOM PANEL
        ImGui::SetNextWindowPos({sideWidth, winSize.y - bottomHeight});
        ImGui::SetNextWindowSize({centerWidth, bottomHeight});
        ImGui::Begin("Bottom", nullptr, flags);
        ImGui::End();

        // RIGHT PANEL (Inspector)
        ImGui::SetNextWindowPos({winSize.x - sideWidth, 0});
        ImGui::SetNextWindowSize({sideWidth, winSize.y - bottomHeight});
        ImGui::Begin("Inspector", nullptr, flags);

        if (selectedEntityIndex >= 0 && selectedEntityIndex < (int)entities.size()) {
            Entity &e = entities[selectedEntityIndex];

            ImGui::Text("Entity Inspector");
            ImGui::Separator();

            ImGui::InputText("Name", &e.name);

            const char* entityTypes[] = {"None", "Enemy", "Player", "Weapon", "Bullet"};
            int currentType = static_cast<int>(e.type);
            if (ImGui::Combo("Type", &currentType, entityTypes, IM_ARRAYSIZE(entityTypes)))
                e.type = static_cast<EntityType>(currentType);

            ImGui::Separator();
            ImGui::Text("Components");

            static int componentToAdd = 0;
            ImGui::Combo("Add Component", &componentToAdd, componentNames, IM_ARRAYSIZE(componentNames));
            ImGui::SameLine();
            if (ImGui::Button("Add")) {
                switch (componentToAdd) {
                    case 0: e.animatedSprite.push_back({}); break;
                    case 1: e.attack.push_back({}); break;
                    case 2: e.clock.push_back({}); break;
                    case 3: e.cooldown.push_back({}); break;
                    case 4: e.direction.push_back({}); break;
                    case 5: e.entitySpawner.push_back({}); break;
                    case 6: e.gravity.push_back({}); break;
                    case 7: e.hitbox.push_back({}); break;
                    case 8: e.hp.push_back({}); break;
                    case 9: e.position.push_back({}); break;
                    case 10: e.sound.push_back({}); break;
                    case 11: e.sprite.push_back({}); break;
                    case 12: e.strategy.push_back({}); break;
                    case 13: e.velocity.push_back({}); break;
                }
            }

            // Draw components
            drawComponentList(e.animatedSprite, "AnimatedSprite", "AS");
            drawComponentList(e.attack, "Attack", "AT");
            drawComponentList(e.clock, "Clock", "CL");
            drawComponentList(e.cooldown, "Cooldown", "CD");
            drawComponentList(e.direction, "Direction", "DIR");
            drawComponentList(e.entitySpawner, "EntitySpawner", "ES");
            drawComponentList(e.gravity, "Gravity", "GR");
            drawComponentList(e.hitbox, "Hitbox", "HB");
            drawComponentList(e.hp, "Hp", "HP");
            drawComponentList(e.position, "Position", "POS");
            drawComponentList(e.sound, "Sound", "SD");
            drawComponentList(e.sprite, "Sprite", "SP");
            drawComponentList(e.strategy, "Strategy", "ST");
            drawComponentList(e.velocity, "Velocity", "VEL");
        } else {
            ImGui::Text("No entity selected");
        }

        ImGui::End();

        // CENTER PANEL
        ImGui::SetNextWindowPos({sideWidth, 0});
        ImGui::SetNextWindowSize({centerWidth, centerHeight});
        ImGui::Begin("Center", nullptr, flags);
        ImGui::Text("Center view");
        ImGui::End();

        window.clear(sf::Color(40, 40, 40));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
