#pragma once

#include "../IComponent.hpp"
#include <SFML/Graphics.hpp>

/**
 * @brief Class Sprite
 */
class Sprite : public IComponent
{
private:
    sf::Texture _texture;
    sf::Sprite _sprite;
    bool _is_visible = true;

public:
    Sprite(const std::string& file_path, float size_x, float size_y);
    ~Sprite() override = default;

    // SET / GET
    sf::Sprite& GetSprite();
    void SetTexture(const std::string& file_path);

    // SIZE
    std::pair<float, float> GetSize() const;
    void SetSize(std::pair<float, float> size);
    void SetSize(float size_x, float size_y);

    void multiplySize(std::pair<float, float> multiplicator);
    void multiplySize(float multiplicatorX, float multiplicatorY);
    void deviseSize(std::pair<float, float> divisor);
    void deviseSize(float divisor_x, float divisor_y);

    // Visibility
    bool GetVisibility() const;
    void SetVisibility(bool visibility);
    void show();
    void hide();
};
