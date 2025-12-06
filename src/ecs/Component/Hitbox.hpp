#pragma once

#include <vector>

#include "../IComponent.hpp"

/**
 * @class Hitbox
 * @brief Represents the Hitbox of a component
 *
 * @param _sizeX The width of the hitbox
 * @param _sizeY The height of the hitbox
 * @param _layers The layers the hitbox belongs to
 * @param _masks The masks the hitbox interacte with
 */
class Hitbox : public IComponent
{
    private:
        float _sizeX;
        float _sizeY;
        std::vector<int> _layers;
        std::vector<int> _masks;

    public:
        Hitbox(float sizeX, float sizeY);
        Hitbox(float sizeX, float sizeY, std::vector<int> layers, std::vector<int> mask);
        ~Hitbox() override = default;

        // Size
        std::pair<float, float> GetHitboxSize();
        void SetHitboxSize(float sizeX, float sizeY);
        void SetHitboxSize(std::pair<float, float> size);

        // Layers
        void AddLayer(int layer);
        void SubLayer(int layer);
        std::vector<int> GetLayers();
        void SetLayers(std::vector<int> layers);

        // Masks
        void AddMask(int mask);
        void SubMask(int mask);
        std::vector<int> GetMask();
        void SetMask(std::vector<int> mask);
};
