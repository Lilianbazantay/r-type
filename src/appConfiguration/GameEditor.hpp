#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

#include <imgui.h>

#include "Entity.hpp"

class GameEditor {
private:
    // =====================
    // Window / ImGui
    // =====================
    sf::RenderWindow window;
    sf::Clock deltaClock;

    ImVec2 winSize{};
    float sideWidth{};
    float bottomHeight{};
    float centerWidth{};
    float centerHeight{};
    std::vector<ParallaxLayer> _paralaxe;

    // =====================
    // Data
    // =====================
    std::vector<Entity> entities;
    int selectedEntityIndex = -1;

    // =====================
    // Textures cache pour le panel central
    // =====================
    std::unordered_map<std::string, sf::Texture> spriteTextures;
    sf::Texture defaultTexture;

    // =====================
    // Core
    // =====================
    void initWindow();
    void processEvents();
    void updateLayout();
    void render();

    // =====================
    // Panels
    // =====================
    void drawTopBar();
    void drawLeftPanel();
    void drawRightPanel();
    void drawBottomPanel();
    void drawCenterPanel();

    // =====================
    // JSON
    // =====================
    void SaveAllEntities();
    void LoadAllEntities();

public:
    void RunGameEditor();
};
