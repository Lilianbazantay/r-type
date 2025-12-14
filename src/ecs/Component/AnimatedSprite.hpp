#pragma once

#include "../IComponent.hpp"

#include <SFML/Graphics.hpp>

class AnimatedSprite : public IComponent
{
private:
    sf::Texture _texture;
    sf::Sprite _sprite;
    sf::IntRect _currentRect;
    int _number_of_sprite_x;
    int _number_of_sprite_y;
    float _animation_rate;
    float _elapsed_time = 0.f;
    int _current_frame_x = 0;
    int _current_frame_y = 0;
    bool _is_visible = true;

public:
    // ANIMATED SPRITE
    AnimatedSprite(const std::string& file_path, float size_x, float size_y, int number_of_sprite_x, int number_of_sprite_y);
    ~AnimatedSprite() override = default;

    // GET / SET
    sf::Sprite& GetSprite();
    void SetTexture(const std::string& file_path);

    // SIZE
    std::pair<float, float> GetSize() const;
    void SetSize(std::pair<float, float> size);
    void SetSize(float size_x, float size_y);
    void multiplySize(std::pair<float, float> multiplicator);
    void multiplySize(float multiplicator_x, float multiplicator_y);
    void deviseSize(std::pair<float, float> divisor);
    void deviseSize(float divisor_x, float divisor_y);

    // NUMBER OF SPRITE
    std::pair<int, int> GetNumberOfSprite() const;
    void SetNumberOfSprite(std::pair<int, int> number_of_sprite);
    void SetNumberOfSprite(int number_of_sprite_x, int number_of_sprite_y);

    // ANIMATION
    float GetAnimationRate() const;
    void SetAnimationRate(float animation_rate);

    // VISIBILITY
    bool GetVisibility() const;
    void SetVisibility(bool visibility);
    void show();
    void hide();

    // UPDATE
    void update(float deltaTime);
};
