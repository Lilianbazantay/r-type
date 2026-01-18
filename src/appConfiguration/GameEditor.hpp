#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

#include <imgui.h>

#include "Entity.hpp"

/**
 * @brief This class contain every data on the game actualy modified
 */
class GameEditor {
private:
    // =====================
    // Window / ImGui
    // =====================
    sf::RenderWindow _window;
    sf::Clock _deltaClock;

    ImVec2 _winSize{};
    float _sideWidth{};
    float _bottomHeight{};
    float _centerWidth{};
    float _centerHeight{};
    std::vector<ParallaxLayer> _paralaxe;

    // =====================
    // Data
    // =====================
    std::vector<Entity> entities;
    int _selectedEntityIndex = -1;
    std::vector<std::string> _log;

    // =====================
    // Textures
    // =====================
    std::unordered_map<std::string, sf::Texture> _spriteTextures;
    sf::Texture _defaultTexture;

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
    void runGameEditor();
};
