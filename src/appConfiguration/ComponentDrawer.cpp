#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>

#include <vector>
#include <string>
#include <utility>

#include "ComponentDrawer.hpp"

static void DrawContextPath(const char* label, std::string& path)
{
    ImGui::InputText(label, &path);
}

// Animated Sprite
static void DrawAnimatedContext(const char* name, AnimatedSpriteContext& ctx)
{
    if (ImGui::TreeNode(name)) {
        ImGui::InputText("Path", &ctx.path);
        ImGui::InputInt("Grid X", &ctx.number_of_sprite_x);
        ImGui::InputInt("Grid Y", &ctx.number_of_sprite_y);
        ImGui::TreePop();
    }
}

void ComponentDrawer::drawComponentList(std::vector<AnimatedSpriteStruct>& vec, const char* label, const char*)
{
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);

        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {

            AnimatedSpriteStruct& a = vec[i];

            ImGui::InputFloat("Size X", &a.size_x);
            ImGui::InputFloat("Size Y", &a.size_y);
            ImGui::InputFloat("Animation Rate", &a.animation_rate);
            ImGui::Checkbox("Visible", &a.is_visible);

            ImGui::Separator();
            ImGui::Text("Contexts");

            DrawAnimatedContext("Idle",  a.idle);
            DrawAnimatedContext("Up",    a.up);
            DrawAnimatedContext("Down",  a.down);
            DrawAnimatedContext("Shoot", a.shoot);
            DrawAnimatedContext("Death", a.death);

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

// Clock
void ComponentDrawer::drawComponentList(std::vector<ClockStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputFloat("Elapsed Time", &vec[i].elapsedTime);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// Cooldown
void ComponentDrawer::drawComponentList(std::vector<CooldownStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputDouble("Length", &vec[i].length);
            ImGui::InputDouble("Remaining", &vec[i].remaining);
            ImGui::InputDouble("Timestamp", &vec[i].timestamp_activation);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// Direction
void ComponentDrawer::drawComponentList(std::vector<DirectionStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputFloat("X", &vec[i].x);
            ImGui::InputFloat("Y", &vec[i].y);
            ImGui::InputFloat("Angle", &vec[i].angle);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// Entity Spawner
void ComponentDrawer::drawComponentList(std::vector<EntitySpawnerStruct>& vec, const std::vector<Entity>& allEntities, const char* label, const char*)
{
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);

        std::string nodeLabel = std::string(label) + " " + std::to_string(i);
        if (ImGui::TreeNode(nodeLabel.c_str())) {

            auto& spawner = vec[i];

            ImGui::Checkbox("Activated", &spawner.is_activated);
            ImGui::InputDouble("Cooldown Length", &spawner.cooldown_length);

            if (allEntities.empty()) {
                ImGui::TextDisabled("No entities available");
                spawner.entityIndex = -1;
            } else {
                const char* preview =
                    (spawner.entityIndex >= 0 &&
                     spawner.entityIndex < (int)allEntities.size())
                        ? allEntities[spawner.entityIndex].name.c_str()
                        : "None";

                if (ImGui::BeginCombo("Entity Spawned", preview)) {
                    for (int e = 0; e < (int)allEntities.size(); ++e) {
                        bool selected = (spawner.entityIndex == e);
                        if (ImGui::Selectable(allEntities[e].name.c_str(), selected)) {
                            spawner.entityIndex = e;
                        }
                        if (selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

// Gravity
void ComponentDrawer::drawComponentList(std::vector<GravityStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::Checkbox("Activated", &vec[i].is_activated);
            ImGui::InputFloat("Attraction Strength", &vec[i].attraction_strength);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// Hitbox
void ComponentDrawer::drawComponentList(std::vector<HitboxStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);

        std::string nodeLabel = std::string(label) + " " + std::to_string(i);
        if (ImGui::TreeNode(nodeLabel.c_str())) {

            HitboxStruct& hitbox = vec[i];

            ImGui::PushItemWidth(120);

            ImGui::InputFloat("Size X", &hitbox.sizeX);
            ImGui::InputFloat("Size Y", &hitbox.sizeY);
            ImGui::InputInt("Damage", &hitbox.damage);

            ImGui::Separator();

            ImGui::Text("Masks");
            for (size_t j = 0; j < hitbox.masks.size(); ++j) {
                ImGui::PushID((int)j);
                ImGui::PushItemWidth(80);
                ImGui::InputInt("##Mask", &hitbox.masks[j]);
                ImGui::SameLine();
                if (ImGui::Button("X##Mask")) {
                    hitbox.masks.erase(hitbox.masks.begin() + j);
                    ImGui::PopID();
                    break;
                }
                ImGui::PopItemWidth();
                ImGui::PopID();
            }

            static int newMask = 0;
            ImGui::PushItemWidth(80);
            ImGui::InputInt("New Mask", &newMask);
            ImGui::SameLine();
            if (ImGui::Button("Add Mask")) {
                hitbox.masks.push_back(newMask);
            }
            ImGui::PopItemWidth();

            ImGui::Separator();

            ImGui::Text("Layers");
            for (size_t j = 0; j < hitbox.layers.size(); ++j) {
                ImGui::PushID((int)j + 1000);
                ImGui::PushItemWidth(80);
                ImGui::InputInt("##Layer", &hitbox.layers[j]);
                ImGui::SameLine();
                if (ImGui::Button("X##Layer")) {
                    hitbox.layers.erase(hitbox.layers.begin() + j);
                    ImGui::PopID();
                    break;
                }
                ImGui::PopItemWidth();
                ImGui::PopID();
            }

            static int newLayer = 0;
            ImGui::PushItemWidth(80);
            ImGui::InputInt("New Layer", &newLayer);
            ImGui::SameLine();
            if (ImGui::Button("Add Layer")) {
                hitbox.layers.push_back(newLayer);
            }
            ImGui::PopItemWidth();

            ImGui::PopItemWidth();

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}


// Hp
void ComponentDrawer::drawComponentList(std::vector<HpStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputInt("Value", &vec[i].value);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// Position
void ComponentDrawer::drawComponentList(std::vector<PositionStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputFloat("X", &vec[i].x);
            ImGui::InputFloat("Y", &vec[i].y);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// Sound
void ComponentDrawer::drawComponentList(std::vector<SoundStruct>& vec, const char* label, const char*)
{
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);

        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {

            SoundStruct& s = vec[i];

            ImGui::InputFloat("Volume", &s.volume);
            ImGui::Checkbox("Looping", &s.is_looping);

            ImGui::Separator();
            ImGui::Text("Contexts");

            DrawContextPath("Idle",  s.idle.path);
            DrawContextPath("Up",    s.up.path);
            DrawContextPath("Down",  s.down.path);
            DrawContextPath("Shoot", s.shoot.path);
            DrawContextPath("Death", s.death.path);

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}


// Sprite
void ComponentDrawer::drawComponentList(std::vector<SpriteStruct>& vec, const char* label, const char*)
{
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);

        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {

            SpriteStruct& s = vec[i];

            ImGui::InputFloat("Size X", &s.size_x);
            ImGui::InputFloat("Size Y", &s.size_y);
            ImGui::Checkbox("Visible", &s.is_visible);

            ImGui::Separator();
            ImGui::Text("Contexts");

            DrawContextPath("Idle",  s.idle.path);
            DrawContextPath("Up",    s.up.path);
            DrawContextPath("Down",  s.down.path);
            DrawContextPath("Shoot", s.shoot.path);
            DrawContextPath("Death", s.death.path);

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

// Strategy
void ComponentDrawer::drawComponentList(std::vector<StrategyStruct>& vec, const char* label, const char*)
{
    static const char* ActionNames[] = {
        "UP", "DOWN", "RIGHT", "LEFT", "SHOOT", "LOOP"
    };

    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID(static_cast<int>(i));

        std::string nodeLabel = std::string(label) + " " + std::to_string(i);
        if (ImGui::TreeNode(nodeLabel.c_str())) {

            StrategyStruct& strat = vec[i];

            ImGui::Text("Actions count: %zu", strat.pattern.size());
            ImGui::Separator();

            for (size_t j = 0; j < strat.pattern.size(); ++j) {
                ImGui::PushID(static_cast<int>(j));

                ImGui::Separator();
                ImGui::Text("Action %zu", j);

                int action = strat.pattern[j].first;
                ImGui::Combo("Type", &action, ActionNames, IM_ARRAYSIZE(ActionNames));
                strat.pattern[j].first = action;

                ImGui::InputInt("Value", &strat.pattern[j].second);

                if (ImGui::Button("Move Up") && j > 0)
                    std::swap(strat.pattern[j], strat.pattern[j - 1]);

                ImGui::SameLine();

                if (ImGui::Button("Move Down") && j + 1 < strat.pattern.size())
                    std::swap(strat.pattern[j], strat.pattern[j + 1]);

                ImGui::SameLine();

                if (ImGui::Button("Delete")) {
                    strat.pattern.erase(strat.pattern.begin() + j);
                    ImGui::PopID();
                    break;
                }

                ImGui::PopID();
            }

            ImGui::Separator();
            ImGui::Text("Add new action");

            static int newAction = 0;
            static int newValue = 0;

            ImGui::Combo("New Type", &newAction, ActionNames, IM_ARRAYSIZE(ActionNames));
            ImGui::InputInt("New Value", &newValue);

            if (ImGui::Button("Add Action")) {
                strat.pattern.emplace_back(newAction, newValue);
            }

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}


// Velocity
void ComponentDrawer::drawComponentList(std::vector<VelocityStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputFloat("Value", &vec[i].value);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}
