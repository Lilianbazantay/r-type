#include "AnimatedSprite.hpp"

/**
 * @brief Construct a new AnimatedSprite:: AnimatedSprite object
 *
 * @param link is the link of the AnimatedSprite
 * @param size_x set the size x
 * @param size_y set the size y
 * @param number_of_spritex set _number_of_sprite_x
 * @param number_of_sprite_y set _number_of_sprite_y
 */
AnimatedSprite::AnimatedSprite(std::string link, float size_x, float size_y, int number_of_sprite_x, int number_of_sprite_y) :
_link(link), _size_x(size_x), _size_y(size_y), _number_of_sprite_x(number_of_sprite_x), _number_of_sprite_y(number_of_sprite_y)
{
    _type = ComponentType::ANIMATED_SPRITE;
}

/**
 * @brief Get the AnimatedSprite link
 *
 * @return std::string return the AnimatedSprite link
 */
std::string AnimatedSprite::GetLink()
{
    return _link;
}

/**
 * @brief Replace _link by a new link
 *
 * @param link new link
 */
void AnimatedSprite::SetLink(std::string link)
{
    _link = link;
}

/**
 * @brief Return the AnimatedSprite size
 *
 * @return std::pair<float, float> First : size_x, Second: size_y
 */
std::pair<float, float> AnimatedSprite::GetSize()
{
    return {_size_x, _size_y};
}

/**
 * @brief Set the AnimatedSprite size with a pair
 *
 * @param size First: size_x, Second: size_y
 */
void AnimatedSprite::SetSize(std::pair<float, float> size)
{
    _size_x = size.first;
    _size_y = size.second;
}

/**
 * @brief Set the AnimatedSprite size
 *
 * @param size_x Set _size_x
 * @param size_y Set _size_y
 */
void AnimatedSprite::SetSize(float size_x, float size_y)
{
    _size_x = size_x;
    _size_y = size_y;
}

/**
 * @brief Return the number of sprite in the animation
 *
 * @return std::pair<int, int> First: number of sprite in x, Second: number of sprite in y
 */
std::pair<int, int> AnimatedSprite::GetNumberOfSprite()
{
    return {_number_of_sprite_x, _number_of_sprite_y};
}

/**
 * @brief Set the number of sprite in x and y
 *
 * @param number_of_sprite First: number of sprite in x, Second: number of sprite in y
 */
void AnimatedSprite::SetNumberOfSprite(std::pair<int, int> number_of_sprite)
{
    _number_of_sprite_x = number_of_sprite.first;
    _number_of_sprite_y = number_of_sprite.second;
}

/**
 * @brief Set the number of sprite in x and y
 *
 * @param number_of_sprite_x new number of sprite in x
 * @param number_of_sprite_y new number of sprite in y
 */
void AnimatedSprite::SetNumberOfSprite(int number_of_sprite_x, int number_of_sprite_y)
{
    _number_of_sprite_x = number_of_sprite_x;
    _number_of_sprite_y = number_of_sprite_y;
}

/**
 * @brief Return the animation rate
 *
 * @return float Current animation rate
 */
float AnimatedSprite::GetAnimationRate()
{
    return _animation_rate;
}

/**
 * @brief Set the animation rate
 *
 * @param animation_rate new animation rate
 */
void AnimatedSprite::SetAnimationRate(float animation_rate)
{
    _animation_rate = animation_rate;
}

/**
 * @brief Multiply the _size_x and _size_y of the AnimatedSprite
 *
 * @param multiplicator value the size is multiply by. First: size_x, Second: size_y
 */
void AnimatedSprite::multiplySize(std::pair<float, float> multiplicator)
{
    _size_x *= multiplicator.first;
    _size_y *= multiplicator.second;
}

/**
 * @brief Multiply the _size_x and _size_y of the AnimatedSprite
 *
 * @param multiplicator_x value _size_x is multiply by
 * @param multiplicator_y value _size_y is multiply by
 */
void AnimatedSprite::multiplySize(float multiplicator_x, float multiplicator_y)
{
    _size_x *= multiplicator_x;
    _size_y *= multiplicator_y;
}

/**
 * @brief Divise the _size_x and _size_y of the AnimatedSprite
 *
 * @param divisor value the size is devise by. First: size_x, Second: size_y
 */
void AnimatedSprite::deviseSize(std::pair<float, float> divisor)
{
    if (divisor.first != 0 && divisor.second != 0) {
        _size_x /= divisor.first;
        _size_y /= divisor.second;
    }
}

/**
 * @brief Divise the _size_x and _size_y of the AnimatedSprite
 *
 * @param divisor_x value _size_x is divise by
 * @param divisor_y value _size_y is divise by
 */
void AnimatedSprite::deviseSize(float divisor_x, float divisor_y)
{
    if (divisor_x != 0 && divisor_y != 0) {
        _size_x /= divisor_x;
        _size_y /= divisor_y;
    }
}

/**
 * @brief Return _is_visible
 *
 * @return true the animates sprite is visible
 * @return false the animates sprite is not visible
 */
bool AnimatedSprite::GetVisibility()
{
    return _is_visible;
}

/**
 * @brief Set the visibility
 *
 * @param visibility Set _is_visible
 */
void AnimatedSprite::SetVisibility(bool visibility)
{
    _is_visible = visibility;
}

/**
 * @brief Set is_visible to true
 */
void AnimatedSprite::show()
{
    _is_visible = true;
}

/**
 * @brief Set is_visible to false
 */
void AnimatedSprite::hide()
{
    _is_visible = false;
}
