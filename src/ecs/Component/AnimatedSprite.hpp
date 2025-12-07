#pragma once

#include "../IComponent.hpp"

#include <string>

/**
 * @brief Class for an AnimatedSprite like a player or ennnemy
 */
class AnimatedSprite : public IComponent
{
private:
    std::string _link;
    float _size_x;
    float _size_y;
    int _number_of_sprite_x;
    int _number_of_sprite_y;
    float _animation_rate;
    bool _is_visible = true;

public:
    AnimatedSprite(std::string link, float size_x, float size_y, int number_of_sprite_x, int number_of_sprite_y);
    ~AnimatedSprite() override = default;

    // SET / GET
    std::string GetLink();
    void SetLink(std::string link);

    std::pair<float, float> GetSize();
    void SetSize(std::pair<float, float> size);
    void SetSize(float size_x, float size_y);

    std::pair<int, int> GetNumberOfSprite();
    void SetNumberOfSprite(std::pair<int, int> number_of_sprite);
    void SetNumberOfSprite(int number_of_sprite_x, int number_of_sprite_y);

    float GetAnimationRate();
    void SetAnimationRate(float animation_rate);
    // OPERATOR
    void multiplySize(std::pair<float, float> multiplicator);
    void multiplySize(float multiplicator_x, float multiplicator_y);
    void deviseSize(std::pair<float, float> divisor);
    void deviseSize(float divisor_x, float divisor_y);

    // VISIBILITY
    void show();
    void hide();
};
