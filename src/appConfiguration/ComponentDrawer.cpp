#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>

#include <vector>
#include <string>
#include <utility>
#include <filesystem>

#include "ComponentDrawer.hpp"

namespace fs = std::filesystem;

/**
 * @brief Display a dropdown button with every asset with the good extension in the given directory
 *
 * @param popup_id Id of the button
 * @param outPath Path of the directory where the are every assets
 * @param startDir Path of the directory where the are every assets set default as "assets"
 * @param extensions every extension supported
 *
 * @return true return true if it work
 * @return false return false if it fail
 */
static bool DrawFileBrowserPopup(const char* popup_id, std::string& outPath, const std::string& startDir, const std::vector<std::string>& extensions)
{
    static fs::path currentDir;
    static bool initialized = false;
    bool selected = false;

    if (!initialized) {
        currentDir = startDir.empty() ? fs::current_path() : fs::path(startDir);
        initialized = true;
    }

    if (ImGui::BeginPopup(popup_id)) {

        ImGui::Text("Current: %s", currentDir.string().c_str());
        ImGui::Separator();

        if (currentDir.has_parent_path()) {
            if (ImGui::Selectable("..")) {
                currentDir = currentDir.parent_path();
            }
        }

        for (auto& entry : fs::directory_iterator(currentDir)) {
            const auto& path = entry.path();
            std::string name = path.filename().string();

            if (entry.is_directory()) {
                if (ImGui::Selectable((name + "/").c_str())) {
                    currentDir /= name;
                }
            }
            else if (entry.is_regular_file()) {
                if (!extensions.empty()) {
                    bool valid = false;
                    for (auto& ext : extensions) {
                        if (path.extension() == ext) {
                            valid = true;
                            break;
                        }
                    }
                    if (!valid)
                        continue;
                }

                if (ImGui::Selectable(name.c_str())) {
                    outPath = path.string();
                    selected = true;
                    ImGui::CloseCurrentPopup();
                }
            }
        }

        ImGui::EndPopup();
    }

    return selected;
}

/**
 * @brief Display the path selector
 *
 * @param label name of the path selector
 * @param path path to store the assets
 * @param extensions type of assets accepted
 * @param startDir place where assets are
 */
void ComponentDrawer::drawPathSelector(const char* label, std::string& path, const std::vector<std::string>& extensions, const std::string& startDir)
{
    ImGui::InputText(label, &path);
    ImGui::SameLine();

    std::string popup = std::string("##browser_") + label;
    if (ImGui::Button(("Browse##" + std::string(label)).c_str())) {
        ImGui::OpenPopup(popup.c_str());
    }

    DrawFileBrowserPopup(popup.c_str(), path, startDir, extensions);
}

// ====================
// Animated Sprite
// ====================

/**
 * @brief Draw the animation sprite context into a tree node
 *
 * @param name name of the context ("idle", "death", "up", "down", "shoot", "hit")
 * @param ctx Contain the structure on animated sprite component
 */
static void DrawAnimatedContext(const char* name, AnimatedSpriteContext& ctx)
{
    if (ImGui::TreeNode(name)) {
        ImGui::InputInt("Lenth", &ctx.lenth);
        ImGui::InputFloat("Size X", &ctx.size.x);
        ImGui::InputFloat("Size Y", &ctx.size.y);
        ImGui::InputInt("Step X", &ctx.step.x);
        ImGui::InputInt("Step Y", &ctx.step.x);
        ImGui::TreePop();
    }
}

/**
 * @brief Drawn every animation sprite of the entity
 *
 * @param vec vector of animated sprite
 * @param label name of the animation
 */
void ComponentDrawer::drawComponentList(std::vector<AnimatedSpriteStruct>& vec, const char* label, const char*)
{
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);

        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {

            ComponentDrawer::drawPathSelector("Path", vec[i].path, { ".png", ".jpg", ".jpeg" });
            ImGui::InputFloat("Animation Rate", &vec[i].animation_rate);
            ImGui::Checkbox("Visible", &vec[i].is_visible);

            ImGui::Text("Contexts");
            DrawAnimatedContext("Idle",  vec[i].idle);
            DrawAnimatedContext("Up",    vec[i].up);
            DrawAnimatedContext("Down",  vec[i].down);
            DrawAnimatedContext("Shoot", vec[i].shoot);
            DrawAnimatedContext("Death", vec[i].death);

            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }

            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

// ====================
// Clock
// ====================
/**
 * @brief Drawn every animation sprite of the entity
 *
 * @param vec vector of animated sprite
 * @param label name of the animation
 */
void ComponentDrawer::drawComponentList(std::vector<ClockStruct> &vec, const char* label, const char*)
{
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::Checkbox("Activate", &vec[i].is_started);
            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// ====================
// Cooldown
// ====================
/**
 * @brief Drawn every cooldown of the entity
 *
 * @param vec vector of cooldown
 * @param label name of the cooldown
 */
void ComponentDrawer::drawComponentList(std::vector<CooldownStruct> &vec, const char* label, const char*)
{
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputDouble("Length", &vec[i].length);
            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// ====================
// Direction
// ====================
/**
 * @brief Drawn every direction of the entity
 *
 * @param vec vector of direction
 * @param label name of the direction
 */
void ComponentDrawer::drawComponentList(std::vector<DirectionStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputFloat("X", &vec[i].vec.x);
            ImGui::InputFloat("Y", &vec[i].vec.y);
            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// ====================
// Entity Spawner
// ====================
/**
 * @brief Drawn every entity spawner of the entity
 *
 * @param vec vector of entity spawner
 * @param allEntities vector of every entity to choose one
 * @param label name of the entity spawner
 */
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
            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

// ====================
// Gravity
// ====================
/**
 * @brief Drawn every gravity of the entity
 *
 * @param vec vector of gravity
 * @param label name of the gravity
 */
void ComponentDrawer::drawComponentList(std::vector<GravityStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputFloat("Strength", &vec[i].strength);
            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// ====================
// Hitbox
// ====================
/**
 * @brief Drawn every hitbox of the entity
 *
 * @param vec vector of hitbox
 * @param label name of the hitbox
 */
void ComponentDrawer::drawComponentList(std::vector<HitboxStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);

        std::string nodeLabel = std::string(label) + " " + std::to_string(i);
        if (ImGui::TreeNode(nodeLabel.c_str())) {

            HitboxStruct& hitbox = vec[i];

            ImGui::PushItemWidth(120);

            ImGui::InputInt("Size X", &hitbox.size.x);
            ImGui::InputInt("Size Y", &hitbox.size.y);
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
            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

// ====================
// Hp
// ====================
/**
 * @brief Drawn every hp of the entity
 *
 * @param vec vector of hp
 * @param label name of the hp
 */
void ComponentDrawer::drawComponentList(std::vector<HpStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputInt("Value", &vec[i].value);
            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// ====================
// Position
// ====================
/**
 * @brief Drawn every position of the entity
 *
 * @param vec vector of position
 * @param label name of the position
 */
void ComponentDrawer::drawComponentList(std::vector<PositionStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputFloat("X", &vec[i].pos.x);
            ImGui::InputFloat("Y", &vec[i].pos.y);
            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// ====================
// Sound
// ====================
/**
 * @brief Draw the sound context into a tree node
 *
 * @param name name of the context ("idle", "death", "up", "down", "shoot", "hit")
 * @param ctx Contain the structure on sound component
 */
static void DrawSoundContext(const char* name, SoundContext& ctx)
{
    if (ImGui::TreeNode(name)) {
        ComponentDrawer::drawPathSelector("Path", ctx.path, { ".wav", ".ogg", ".mp3" });
        ImGui::InputFloat("Volume", &ctx.volume);
        ImGui::Checkbox("loop", &ctx.loop);
        ImGui::TreePop();
    }
}

/**
 * @brief Drawn every sound of the entity
 *
 * @param vec vector of sound
 * @param label name of the sound
 */
void ComponentDrawer::drawComponentList(std::vector<SoundStruct>& vec, const char* label, const char*)
{
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);

        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::Text("Context");
            DrawSoundContext("Idle", vec[i].idle);
            DrawSoundContext("Up", vec[i].up);
            DrawSoundContext("Down", vec[i].down);
            DrawSoundContext("Shoot", vec[i].shoot);
            DrawSoundContext("Hit", vec[i].hit);
            DrawSoundContext("Death", vec[i].death);
            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}

// ====================
// Sprite
// ====================
/**
 * @brief Drawn every sprite of the entity
 *
 * @param vec vector of sprite
 * @param label name of the sprite
 */
void ComponentDrawer::drawComponentList(std::vector<SpriteStruct>& vec, const char* label, const char*)
{
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);

        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputFloat("Size X", &vec[i].size_x);
            ImGui::InputFloat("Size Y", &vec[i].size_y);
            ImGui::Checkbox("Visible", &vec[i].is_visible);
            ComponentDrawer::drawPathSelector("Path", vec[i].path, { ".png", ".jpg", ".jpeg" });
            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

// ====================
// Strategy
// ====================
/**
 * @brief Draw a vector of two integer
 *
 * @param label name of the vector
 * @param vec vector
 */
inline void DrawVec2i(const char* label, sf::Vector2i& vec) {
    int v[2] = { vec.x, vec.y };
    if (ImGui::InputInt2(label, v)) {
        vec.x = v[0];
        vec.y = v[1];
    }
}

/**
 * @brief Draw the pattern (an action with a value)
 *
 * @param pattern pattern to store the action and value
 * @param pattern_index index of the pattern to delete
 */
inline void drawPattern(Pattern& pattern, int pattern_index) {
    ImGui::Separator();
    DrawVec2i(("HP Range##" + std::to_string(pattern_index)).c_str(), pattern.hp_range);

    int move_index = 0;
    for (auto& move : pattern.movements) {
        ImGui::PushID(move_index);
        ImGui::Text("Move %d", move_index);

        static const char* move_names[] = { "UP", "DOWN", "RIGHT", "UP_RIGHT", "DOWN_RIGHT", "LEFT", "UP_LEFT", "DOWN_LEFT", "SHOOT", "LOOP" };
        int current_index = 0;
        for (int i = 0; i < 6; ++i) {
            if (StrategyDictionary.at(move_names[i]) == move.direction) {
                current_index = i;
                break;
            }
        }

        std::string combo_label = "Type##" + std::to_string(pattern_index) + "_" + std::to_string(move_index);
        if (ImGui::Combo(combo_label.c_str(), &current_index, move_names, IM_ARRAYSIZE(move_names))) {
            move.direction = StrategyDictionary.at(move_names[current_index]);
        }

        std::string input_label = "Value##" + std::to_string(pattern_index) + "_" + std::to_string(move_index);
        ImGui::InputFloat(input_label.c_str(), &move.value);

        std::string button_label = "Remove Move##" + std::to_string(pattern_index) + "_" + std::to_string(move_index);
        if (ImGui::Button(button_label.c_str())) {
            pattern.movements.erase(pattern.movements.begin() + move_index);
            ImGui::PopID();
            break;
        }

        ImGui::PopID();
        move_index++;
        ImGui::Separator();
    }

    if (ImGui::Button(("Add Move##" + std::to_string(pattern_index)).c_str())) {
        Move new_move;
        new_move.direction = StrategyDictionary.at("UP");
        new_move.value = 1;
        pattern.movements.push_back(new_move);
    }
}

/**
 * @brief Drawn every strategy of the entity
 *
 * @param vec vector of strategy
 * @param label name of the strategy
 */
void ComponentDrawer::drawComponentList(std::vector<StrategyStruct>& vec, const char* label, const char*)
{
    if (vec.empty())
        return;

    for (size_t strat_index = 0; strat_index < vec.size(); ++strat_index) {
        StrategyStruct& strategy = vec[strat_index];
        ImGui::PushID((int)strat_index);

        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(strat_index)).c_str())) {

            for (size_t pattern_index = 0; pattern_index < strategy.strategy.size(); ++pattern_index) {
                Pattern& pattern = strategy.strategy[pattern_index];
                ImGui::PushID((int)pattern_index);
                drawPattern(pattern, (int)pattern_index);
                ImGui::PopID();
            }

            if (ImGui::Button(("Add Pattern##" + std::to_string(strat_index)).c_str())) {
                strategy.strategy.push_back(Pattern{});
            }

            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + strat_index);
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }

        ImGui::PopID();
    }
}

// ====================
// Velocity
// ====================
/**
 * @brief Drawn every velocity of the entity
 *
 * @param vec vector of velocity
 * @param label name of the velocity
 */
void ComponentDrawer::drawComponentList(std::vector<VelocityStruct> &vec, const char* label, const char*) {
    for (size_t i = 0; i < vec.size(); ++i) {
        ImGui::PushID((int)i);
        if (ImGui::TreeNode((std::string(label) + " " + std::to_string(i)).c_str())) {
            ImGui::InputFloat("Value", &vec[i].value);
            if (ImGui::Button("Delete")) {
                vec.erase(vec.begin() + i);
                ImGui::TreePop();
                ImGui::PopID();
                break;
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
}
