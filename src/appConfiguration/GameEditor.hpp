#pragma once

#include <SFML/Graphics.hpp>
#include <imgui.h>

#include <vector>
#include <string>
#include "Entity.hpp"

class GameEditor
{
private:
    sf::RenderWindow window;
    sf::Clock deltaClock;

    std::vector<Entity> entities;
    int selectedEntityIndex = -1;

    // Layout
    ImVec2 winSize {0.f, 0.f};
    float sideWidth = 0.f;
    float bottomHeight = 0.f;
    float centerWidth = 0.f;
    float centerHeight = 0.f;

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse;

    // Core
    void initWindow();
    void processEvents();
    void updateLayout();
    void render();

    // Panels
    void drawTopBar();
    void drawLeftPanel();
    void drawRightPanel();
    void drawBottomPanel();
    void drawCenterPanel();

    // JSON
    void SaveAllEntities();
    void LoadAllEntities();

public:
    GameEditor() = default;
    ~GameEditor() = default;

    void RunGameEditor();
};
