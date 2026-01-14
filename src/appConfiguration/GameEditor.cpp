#include "GameEditor.hpp"

#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "ComponentDrawer.hpp"
#include "JsonUtils.hpp"

namespace fs = std::filesystem;

// =====================
// Utils
// =====================
static const char* componentNames[] = {
    "AnimatedSprite","Clock","Cooldown",
    "Direction","EntitySpawner","Gravity","Hitbox",
    "Hp","Position","Sound","Sprite",
    "Strategy","Velocity"
};

ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

// =====================
// Public
// =====================
void GameEditor::RunGameEditor()
{
    initWindow();

    while (window.isOpen()) {
        processEvents();
        ImGui::SFML::Update(window, deltaClock.restart());

        updateLayout();

        drawTopBar();
        drawLeftPanel();
        drawBottomPanel();
        drawRightPanel();
        drawCenterPanel();

        render();
    }

    ImGui::SFML::Shutdown();
}

// =====================
// JSON Save / Load
// =====================
void GameEditor::SaveAllEntities()
{
    try {
        for (auto& e : entities) {
            std::string folder;
            switch (e.type) {
                case EntityType::player: folder = "src/appConfiguration/configuration/player/"; break;
                case EntityType::enemy:  folder = "src/appConfiguration/configuration/ennemy/"; break;
                case EntityType::weapon: folder = "src/appConfiguration/configuration/weapon/"; break;
                case EntityType::bullet: folder = "src/appConfiguration/configuration/weapon/bullet/"; break;
                default: folder = "src/appConfiguration/configuration/entity/"; break;
            }
            fs::create_directories(folder);
            std::string filepath = folder + e.name + ".json";

            json jEntity = e;
            std::ofstream file(filepath);
            if (!file.is_open())
                throw std::runtime_error("Cannot open file " + filepath);
            file << jEntity.dump(4);
        }
        std::cout << "All entities saved individually.\n";
    } catch (const std::exception& ex) {
        std::cerr << "SaveAllEntities error: " << ex.what() << "\n";
    }
}

void GameEditor::LoadAllEntities()
{
    try {
        entities.clear();

        // <-- ici aussi on met les minuscules
        std::vector<std::pair<EntityType, std::string>> folders = {
            {EntityType::player, "src/appConfiguration/configuration/player/"},
            {EntityType::enemy,  "src/appConfiguration/configuration/ennemy/"},
            {EntityType::weapon, "src/appConfiguration/configuration/weapon/"},
            {EntityType::bullet, "src/appConfiguration/configuration/weapon/bullet/"}
        };

        for (auto& [type, folder] : folders) {
            if (!fs::exists(folder)) continue;

            for (auto& entry : fs::directory_iterator(folder)) {
                if (entry.is_regular_file() && entry.path().extension() == ".json") {
                    std::ifstream file(entry.path());
                    if (!file.is_open()) {
                        std::cerr << "Failed to open " << entry.path() << "\n";
                        continue;
                    }

                    json jEntity;
                    file >> jEntity;
                    Entity e = jEntity.get<Entity>();
                    entities.push_back(e);
                }
            }
        }

        selectedEntityIndex = -1;
        std::cout << "Loaded all entities from configuration folders.\n";
    } catch (const std::exception& e) {
        std::cerr << "LoadAllEntities error: " << e.what() << "\n";
    }
}

// =====================
// Init
// =====================
void GameEditor::initWindow()
{
    window.create(sf::VideoMode(1920, 1080), "[Insert Name] Editor");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        throw std::runtime_error("Failed to init ImGui-SFML");
    }

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();

    io.FontDefault = io.Fonts->AddFontFromFileTTF(
        "assets/Roboto-Light.ttf",
        25.0f
    );

    if (!io.FontDefault) {
        throw std::runtime_error("Failed to load ImGui font");
    }

    if (!ImGui::SFML::UpdateFontTexture()) {
        throw std::runtime_error("Failed to update ImGui font texture");
    }

    ImGui::GetStyle().ScaleAllSizes(1.15f);
}

// =====================
// Events
// =====================
void GameEditor::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

// =====================
// Layout
// =====================
void GameEditor::updateLayout()
{
    winSize = ImGui::GetIO().DisplaySize;

    sideWidth    = winSize.x * 0.2f;
    bottomHeight = winSize.y * 0.2f;
    centerWidth  = winSize.x - sideWidth * 2.f;
    centerHeight = winSize.y - bottomHeight;
}

// =====================
// Panels
// =====================
void GameEditor::drawTopBar()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(winSize.x, 40));
    ImGui::Begin("TopBar", nullptr,
                 ImGuiWindowFlags_NoDecoration |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoSavedSettings);

    if (ImGui::Button("Save")) {
        SaveAllEntities();
    }
    ImGui::SameLine();
    if (ImGui::Button("Import")) {
        LoadAllEntities();
    }

    ImGui::End();
}

void GameEditor::drawLeftPanel()
{
    ImGui::SetNextWindowPos(ImVec2(0.f, 40.f));
    ImGui::SetNextWindowSize(ImVec2(sideWidth, winSize.y - 40.f));
    ImGui::Begin("Entities", nullptr, windowFlags);

    ImGui::Columns(2, nullptr, false);

    if (ImGui::Button("Add Entity"))
        entities.push_back(Entity{});

    ImGui::Separator();

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
        ImGui::Separator();
    }

    ImGui::Columns(1);
    ImGui::End();
}

void GameEditor::drawRightPanel()
{
    ImGui::SetNextWindowPos(ImVec2(winSize.x - sideWidth, 40.f));
    ImGui::SetNextWindowSize(ImVec2(sideWidth, winSize.y - bottomHeight - 40.f));
    ImGui::Begin("Inspector", nullptr, windowFlags);

    if (selectedEntityIndex < 0 || selectedEntityIndex >= (int)entities.size()) {
        ImGui::Text("No entity selected");
        ImGui::End();
        return;
    }

    Entity& e = entities[selectedEntityIndex];

    ImGui::Text("Entity Inspector");
    ImGui::Separator();

    ImGui::InputText("Name", &e.name);

    const char* entityTypes[] = { "None", "Enemy", "Player", "Weapon", "Bullet" };
    int currentType = static_cast<int>(e.type);
    if (ImGui::Combo("Type", &currentType, entityTypes, IM_ARRAYSIZE(entityTypes)))
        e.type = static_cast<EntityType>(currentType);

    ImGui::Separator();
    ImGui::Text("Components");

    static int componentToAdd = 0;
    ImGui::Combo("", &componentToAdd, componentNames, IM_ARRAYSIZE(componentNames));
    ImGui::SameLine();

    if (ImGui::Button("Add")) {
        switch (componentToAdd) {
            case 0:  e.animatedSprite.push_back({}); break;
            case 1:  e.clock.push_back({}); break;
            case 2:  e.cooldown.push_back({}); break;
            case 3:  e.direction.push_back({}); break;
            case 4:  e.entitySpawner.push_back({}); break;
            case 5:  e.gravity.push_back({}); break;
            case 6:  e.hitbox.push_back({}); break;
            case 7:  e.hp.push_back({}); break;
            case 8:  e.position.push_back({}); break;
            case 9: e.sound.push_back({}); break;
            case 10: e.sprite.push_back({}); break;
            case 11: e.strategy.push_back({}); break;
            case 12: e.velocity.push_back({}); break;
        }
    }

    ComponentDrawer::drawComponentList(e.animatedSprite, "AnimatedSprite", "AS");
    ComponentDrawer::drawComponentList(e.clock, "Clock", "CL");
    ComponentDrawer::drawComponentList(e.cooldown, "Cooldown", "CD");
    ComponentDrawer::drawComponentList(e.direction, "Direction", "DIR");
    ComponentDrawer::drawComponentList(e.entitySpawner, entities, "EntitySpawner", "ES");
    ComponentDrawer::drawComponentList(e.gravity, "Gravity", "GR");
    ComponentDrawer::drawComponentList(e.hitbox, "Hitbox", "HB");
    ComponentDrawer::drawComponentList(e.hp, "Hp", "HP");
    ComponentDrawer::drawComponentList(e.position, "Position", "POS");
    ComponentDrawer::drawComponentList(e.sound, "Sound", "SD");
    ComponentDrawer::drawComponentList(e.sprite, "Sprite", "SP");
    ComponentDrawer::drawComponentList(e.strategy, "Strategy", "ST");
    ComponentDrawer::drawComponentList(e.velocity, "Velocity", "VEL");

    ImGui::End();
}

void GameEditor::drawBottomPanel()
{
    ImGui::SetNextWindowPos(ImVec2(sideWidth, winSize.y - bottomHeight));
    ImGui::SetNextWindowSize(ImVec2(centerWidth, bottomHeight));
    ImGui::Begin("Bottom", nullptr, windowFlags);
    ImGui::End();
}

void GameEditor::drawCenterPanel()
{
    ImGui::SetNextWindowPos(ImVec2(sideWidth, 40.f));
    ImGui::SetNextWindowSize(ImVec2(centerWidth, centerHeight - 40.f));
    ImGui::Begin("Center", nullptr, windowFlags);
    ImGui::Text("Center view");
    ImGui::End();
}

// =====================
// Render
// =====================
void GameEditor::render()
{
    window.clear(sf::Color(40, 40, 40));
    ImGui::SFML::Render(window);
    window.display();
}