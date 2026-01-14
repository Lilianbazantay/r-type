#include "GameEditor.hpp"

#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_stdlib.h>

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

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

static std::string getCurrentDateTime()
{
    std::time_t now = std::time(nullptr);
    std::tm localTime = *std::localtime(&now);

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << localTime.tm_mday << "/"
        << std::setw(2) << std::setfill('0') << (localTime.tm_mon + 1) << "/"
        << (localTime.tm_year + 1900) << " - "
        << std::setw(2) << std::setfill('0') << localTime.tm_hour << ":"
        << std::setw(2) << std::setfill('0') << localTime.tm_min << " : ";

    return oss.str();
}


// =====================
// Public
// =====================
void GameEditor::RunGameEditor()
{
    initWindow();

    while (_window.isOpen()) {
        processEvents();
        ImGui::SFML::Update(_window, _deltaClock.restart());

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
        std::vector<std::pair<EntityType, std::string>> folders = {
            {EntityType::player, "src/appConfiguration/configuration/player/"},
            {EntityType::enemy,  "src/appConfiguration/configuration/ennemy/"},
            {EntityType::weapon, "src/appConfiguration/configuration/weapon/"},
            {EntityType::bullet, "src/appConfiguration/configuration/weapon/bullet/"},
            {EntityType::map, "src/appConfiguration/configuration/map/"},
            {EntityType::none, "src/appConfiguration/configuration/entity/"}
        };

        for (auto& [type, folder] : folders) {
            if (fs::exists(folder)) {
                fs::remove_all(folder);
            }
            fs::create_directories(folder);
        }

        for (auto& e : entities) {
            std::string folder;
            switch (e.type) {
                case EntityType::player: folder = "src/appConfiguration/configuration/player/"; break;
                case EntityType::enemy:  folder = "src/appConfiguration/configuration/ennemy/"; break;
                case EntityType::weapon: folder = "src/appConfiguration/configuration/weapon/"; break;
                case EntityType::bullet: folder = "src/appConfiguration/configuration/weapon/bullet/"; break;
                case EntityType::map: folder = "src/appConfiguration/configuration/map/"; break;
                default: folder = "src/appConfiguration/configuration/entity/"; break;
            }

            std::string filepath = folder + e.name + ".json";
            json jEntity = e;
            std::ofstream file(filepath);
            if (!file.is_open())
                throw std::runtime_error("Cannot open file " + filepath);
            file << jEntity.dump(4);
        }

        std::cout << "All entities saved and configuration folders reset.\n";

    } catch (const std::exception& ex) {
        std::cerr << "SaveAllEntities error: " << ex.what() << "\n";
    }
}


void GameEditor::LoadAllEntities()
{
    try {
        entities.clear();

        std::vector<std::pair<EntityType, std::string>> folders = {
            {EntityType::player, "src/appConfiguration/configuration/player/"},
            {EntityType::enemy,  "src/appConfiguration/configuration/enemy/"},
            {EntityType::weapon, "src/appConfiguration/configuration/weapon/"},
            {EntityType::bullet, "src/appConfiguration/configuration/weapon/bullet/"},
            {EntityType::map, "src/appConfiguration/configuration/map/"}
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

        _selectedEntityIndex = -1;
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
    _window.create(sf::VideoMode(1920, 1080), "[Insert Name] Editor");
    _window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(_window)) {
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
    while (_window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);
        if (event.type == sf::Event::Closed)
            _window.close();
    }
}

// =====================
// Layout
// =====================
void GameEditor::updateLayout()
{
    _winSize = ImGui::GetIO().DisplaySize;

    _sideWidth    = _winSize.x * 0.2f;
    _bottomHeight = _winSize.y * 0.2f;
    _centerWidth  = _winSize.x - _sideWidth * 2.f;
    _centerHeight = _winSize.y - _bottomHeight;
}

// =====================
// Panels
// =====================
void GameEditor::drawTopBar()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(_winSize.x, 40));
    ImGui::Begin("TopBar", nullptr,
                 ImGuiWindowFlags_NoDecoration |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoSavedSettings);

    if (ImGui::Button("Save")) {
        SaveAllEntities();
        _log.push_back("Save the data");
    }
    ImGui::SameLine();
    if (ImGui::Button("Import")) {
        LoadAllEntities();
        _log.push_back("Load the data");
    }

    ImGui::End();
}

void GameEditor::drawLeftPanel()
{
    ImGui::SetNextWindowPos(ImVec2(0.f, 40.f));
    ImGui::SetNextWindowSize(ImVec2(_sideWidth, _winSize.y - 40.f));
    ImGui::Begin("Entities", nullptr, windowFlags);

    ImGui::Columns(2, nullptr, false);

    if (ImGui::Button("Add Entity")) {
        entities.push_back(Entity{});
        _log.push_back("Create a new entity");
    }

    ImGui::Separator();

    for (int i = 0; i < (int)entities.size(); i++) {
        ImGui::PushID(i);

        if (ImGui::Selectable(entities[i].name.c_str(), _selectedEntityIndex == i))
            _selectedEntityIndex = i;

        ImGui::NextColumn();

        if (ImGui::Button("X")) {
            entities.erase(entities.begin() + i);
            _selectedEntityIndex = -1;
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
    ImGui::SetNextWindowPos(ImVec2(_winSize.x - _sideWidth, 40.f));
    ImGui::SetNextWindowSize(ImVec2(_sideWidth, _winSize.y - _bottomHeight - 40.f));
    ImGui::Begin("Inspector", nullptr, windowFlags);

    if (_selectedEntityIndex < 0 || _selectedEntityIndex >= (int)entities.size()) {
        ImGui::Text("No entity selected");
        ImGui::End();
        return;
    }

    Entity& e = entities[_selectedEntityIndex];

    ImGui::Text("Entity Inspector");
    ImGui::Separator();

    ImGui::InputText("Name", &e.name);

    const char* entityTypes[] = { "None", "Enemy", "Player", "Weapon", "Map", "Bullet" };
    int currentType = static_cast<int>(e.type);
    if (ImGui::Combo("Type", &currentType, entityTypes, IM_ARRAYSIZE(entityTypes))) {
        e.type = static_cast<EntityType>(currentType);
    }

    // ====================
    // Bloc Map Parallax
    // ====================
    if (e.type == EntityType::map) {
        ImGui::Separator();
        ImGui::Text("Parallax");

        for (size_t i = 0; i < e._paralaxe.size(); ++i) {
            auto& layer = e._paralaxe[i];
            ImGui::PushID((int)i);

            ImGui::InputText("Path", &layer.path);
            ImGui::InputFloat("Speed", &layer.speed);
            ImGui::SameLine();
            if (ImGui::Button("X")) {
                e._paralaxe.erase(e._paralaxe.begin() + i);
                ImGui::PopID();
                break;
            }
            ImGui::PopID();
            ImGui::Separator();
        }
        if (ImGui::Button("Add Layer")) {
            e._paralaxe.push_back({ "", 0.f });
            _log.push_back("Add a Layer to the map:" + e.name);
        }
    }

    ImGui::Separator();
    ImGui::Text("Components");

    static int componentToAdd = 0;
    ImGui::Combo("", &componentToAdd, componentNames, IM_ARRAYSIZE(componentNames));
    ImGui::SameLine();

    if (ImGui::Button("Add")) {
        switch (componentToAdd) {
            case 0:
                 e.animatedSprite.push_back({});
                 _log.push_back("Add a component: AnimatedSprite to the entity: " + e.name);
                 break;
            case 1:
                 e.clock.push_back({});
                 _log.push_back("Add a component: Clock to the entity: " + e.name);
                 break;
            case 2:
                 e.cooldown.push_back({});
                 _log.push_back("Add a component: Cooldown to the entity: " + e.name);
                 break;
            case 3:
                 e.direction.push_back({});
                 _log.push_back("Add a component: Direction to the entity: " + e.name);
                 break;
            case 4:
                 e.entitySpawner.push_back({});
                 _log.push_back("Add a component: EntitySpawner to the entity: " + e.name);
                 break;
            case 5:
                 e.gravity.push_back({});
                 _log.push_back("Add a component: Gravity to the entity: " + e.name);
                 break;
            case 6:
                 e.hitbox.push_back({});
                 _log.push_back("Add a component: Hitbox to the entity: " + e.name);
                 break;
            case 7:
                 e.hp.push_back({});
                 _log.push_back("Add a component: Hp to the entity: " + e.name);
                 break;
            case 8:
                 e.position.push_back({});
                 _log.push_back("Add a component: Position to the entity: " + e.name);
                 break;
            case 9:
                e.sound.push_back({});
                _log.push_back("Add a component: Sound to the entity: " + e.name);
                break;
            case 10:
                e.sprite.push_back({});
                _log.push_back("Add a component: Sprite to the entity: " + e.name);
                break;
            case 11:
                e.strategy.push_back({});
                _log.push_back("Add a component: Strategy to the entity: " + e.name);
                break;
            case 12:
                e.velocity.push_back({});
                _log.push_back("Add a component: Velocity to the entity: " + e.name);
                break;
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
    ImGui::SetNextWindowPos(ImVec2(_sideWidth, _winSize.y - _bottomHeight));
    ImGui::SetNextWindowSize(ImVec2(_centerWidth, _bottomHeight));
    ImGui::Begin("Bottom", nullptr, windowFlags);

    std::string dateTime = getCurrentDateTime();
    for (size_t i = 0; i < _log.size(); i++) {
        std::string text = dateTime + " - " + _log[i];
        ImGui::Text("%s", text.c_str());
    }

    ImGui::End();
}

void GameEditor::drawCenterPanel()
{
    ImGui::SetNextWindowPos(ImVec2(_sideWidth, 40.f));
    ImGui::SetNextWindowSize(ImVec2(_centerWidth, _centerHeight - 40.f));
    ImGui::Begin("Center", nullptr, windowFlags);

    if (_selectedEntityIndex < 0 || _selectedEntityIndex >= (int)entities.size()) {
        ImGui::Text("No entity selected");
        ImGui::End();
        return;
    }

    Entity& e = entities[_selectedEntityIndex];

    if (_defaultTexture.getSize().x == 0) {
        if (!_defaultTexture.loadFromFile("assets/default.png")) {
            _defaultTexture.create(100, 100);
        }
    }

    ImGui::Text("Visualisation");
    ImGui::Separator();

    // -----------------------------
    // Sprites
    // -----------------------------
    if (!e.sprite.empty()) {
        ImGui::Text("Sprites:");
        for (size_t i = 0; i < e.sprite.size(); ++i) {
            SpriteStruct& s = e.sprite[i];

            if (_spriteTextures.find(s.path) == _spriteTextures.end()) {
                sf::Texture tex;
                if (!tex.loadFromFile(s.path)) {
                    tex = _defaultTexture;
                }
                _spriteTextures[s.path] = tex;
            }

            sf::Texture& tex = _spriteTextures[s.path];
            ImVec2 size(s.size_x > 0 ? s.size_x : tex.getSize().x,
                        s.size_y > 0 ? s.size_y : tex.getSize().y);

            ImGui::PushID((int)i);
            ImGui::Image(tex, size);
            ImGui::PopID();
        }
    } else {
        ImGui::Text("No sprites, using default:");
        ImGui::Image(_defaultTexture, ImVec2(100, 100));
    }

    // -----------------------------
    // AnimatedSprites
    // -----------------------------
    if (!e.animatedSprite.empty()) {
        ImGui::Separator();
        ImGui::Text("Animated Sprites:");

        for (size_t i = 0; i < e.animatedSprite.size(); ++i) {
            AnimatedSpriteStruct& a = e.animatedSprite[i];

            if (_spriteTextures.find(a.idle.path) == _spriteTextures.end()) {
                sf::Texture tex;
                if (!tex.loadFromFile(a.idle.path))
                    tex = _defaultTexture;
                _spriteTextures[a.idle.path] = tex;
            }

            sf::Texture& tex = _spriteTextures[a.idle.path];
            sf::IntRect rect(a.idle.start.x, a.idle.start.y, a.idle.size.x, a.idle.size.y);

            ImGui::Text("AnimatedSprite %zu:", i);
            ImGui::Image(tex, ImVec2((float)rect.width, (float)rect.height));
        }
    }

    // -----------------------------
    // Parallax Layers
    // -----------------------------
    if (e.type == EntityType::map && !e._paralaxe.empty()) {
        ImGui::Separator();
        ImGui::Text("Parallax Layers (superposed):");

        float previewHeight = 100.f;

        for (size_t i = 0; i < e._paralaxe.size(); ++i) {
            auto& layer = e._paralaxe[i];

            if (_spriteTextures.find(layer.path) == _spriteTextures.end()) {
                sf::Texture tex;
                if (!tex.loadFromFile(layer.path)) {
                    tex = _defaultTexture;
                }
                _spriteTextures[layer.path] = tex;
            }

            sf::Texture& tex = _spriteTextures[layer.path];

            ImGui::Text("Layer %zu (speed %.2f):", i, layer.speed);

            float scaleX = _centerWidth / tex.getSize().x;
            float scaleY = previewHeight / tex.getSize().y;

            ImGui::Image(tex, ImVec2(tex.getSize().x * scaleX, tex.getSize().y * scaleY));
        }
    }

    ImGui::End();
}



// =====================
// Render
// =====================
void GameEditor::render()
{
    _window.clear(sf::Color(40, 40, 40));
    ImGui::SFML::Render(_window);
    _window.display();
}