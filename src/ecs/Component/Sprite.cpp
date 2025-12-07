#include "Sprite.hpp"

/**
 * @brief Construct a new Sprite:: Sprite object
 *
 * @param link is the link of the sprite
 * @param size_x set the size x
 * @param size_y set the size y
 */
Sprite::Sprite(std::string link, float size_x, float size_y): _link(link), _size_x(size_x), _size_y(size_y)
{
    _type = ComponentType::SPRITE;
}

/**
 * @brief Get the sprite link
 *
 * @return std::string return the sprite link
 */
std::string Sprite::GetLink()
{
    return _link;
}

/**
 * @brief Replace _link by a new link
 *
 * @param link new link
 */
void Sprite::SetLink(std::string link)
{
    _link = link;
}

/**
 * @brief Return the sprite size
 *
 * @return std::pair<float, float> First : size_x, Second: size_y
 */
std::pair<float, float> Sprite::GetSize()
{
    return {_size_x, _size_y};
}

/**
 * @brief Set the sprite size with a pair
 *
 * @param size First: size_x, Second: size_y
 */
void Sprite::SetSize(std::pair<float, float> size)
{
    _size_x = size.first;
    _size_y = size.second;
}

/**
 * @brief Set the Sprite size
 *
 * @param size_x Set _size_x
 * @param size_y Set _size_y
 */
void Sprite::SetSize(float size_x, float size_y)
{
    _size_x = size_x;
    _size_y = size_y;
}

/**
 * @brief Multiply the _size_x and _size_y of the sprite
 *
 * @param multiplicator value the size is multiply by. First: size_x, Second: size_y
 */
void Sprite::multiplySize(std::pair<float, float> multiplicator)
{
    _size_x *= multiplicator.first;
    _size_y *= multiplicator.second;
}

/**
 * @brief Multiply the _size_x and _size_y of the sprite
 *
 * @param multiplicator_x value _size_x is multiply by
 * @param multiplicator_y value _size_y is multiply by
 */
void Sprite::multiplySize(float multiplicator_x, float multiplicator_y)
{
    _size_x *= multiplicator_x;
    _size_y *= multiplicator_y;
}

/**
 * @brief Divise the _size_x and _size_y of the sprite
 *
 * @param divisor value the size is devise by. First: size_x, Second: size_y
 */
void Sprite::deviseSize(std::pair<float, float> divisor)
{
    if (divisor.first != 0 && divisor.second != 0) {
        _size_x /= divisor.first;
        _size_y /= divisor.second;
    }
}

/**
 * @brief Divise the _size_x and _size_y of the sprite
 *
 * @param divisor_x value _size_x is divise by
 * @param divisor_y value _size_y is divise by
 */
void Sprite::deviseSize(float divisor_x, float divisor_y)
{
    if (divisor_x != 0 && divisor_y != 0) {
        _size_x /= divisor_x;
        _size_y /= divisor_y;
    }
}

/**
 * @brief Return _is_visible
 *
 * @return true the sprite is visible
 * @return false the sprite is not visible
 */
bool Sprite::GetVisibility()
{
    return _is_visible;
}

/**
 * @brief Set the visibility
 *
 * @param visibility Set _is_visible
 */
void Sprite::SetVisibility(bool visibility)
{
    _is_visible = visibility;
}

/**
 * @brief Set is_visible to true
 */
void Sprite::show()
{
    _is_visible = true;
}

/**
 * @brief Set is_visible to false
 */
void Sprite::hide()
{
    _is_visible = false;
}
