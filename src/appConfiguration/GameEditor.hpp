#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include <imgui.h> // <<< OBLIGATOIRE pour ImVec2

#include "Entity.hpp"

class GameEditor {
public:
    void RunGameEditor();

private:
    // =====================
    // Window / ImGui
    // =====================
    sf::RenderWindow window;
    sf::Clock deltaClock;

    ImVec2 winSize{};   // <-- maintenant connu
    float sideWidth{};
    float bottomHeight{};
    float centerWidth{};
    float centerHeight{};

    // =====================
    // Data
    // =====================
    std::vector<Entity> entities;
    int selectedEntityIndex = -1;

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
};
