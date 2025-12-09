#include "Sprite.hpp"
#include <stdexcept>

/**
 * @brief Construct a new Sprite object
 *
 * @param file_path Path to the texture file
 * @param size_x Initial width
 * @param size_y Initial height
 */
Sprite::Sprite(const std::string& file_path, float size_x, float size_y)
{
    if (!_texture.loadFromFile(file_path))
        throw std::runtime_error("Failed to load texture: " + file_path);

    _sprite.setTexture(_texture);
    _sprite.setScale(size_x / _texture.getSize().x, size_y / _texture.getSize().y);
    _type = ComponentType::SPRITE;
}

/**
 * @brief Get the SFML sprite
 *
 * @return sf::Sprite& Reference to the sprite
 */
sf::Sprite& Sprite::GetSprite()
{
    return _sprite;
}

/**
 * @brief Set a new texture from file
 *
 * @param file_path Path to the new texture file
 */
void Sprite::SetTexture(const std::string& file_path)
{
    if (!_texture.loadFromFile(file_path))
        throw std::runtime_error("Failed to load texture: " + file_path);
    _sprite.setTexture(_texture, true);
}

/**
 * @brief Return the sprite size
 *
 * @return std::pair<float, float> First: width, Second: height
 */
std::pair<float, float> Sprite::GetSize() const
{
    sf::Vector2f scale = _sprite.getScale();
    sf::Vector2u texSize = _texture.getSize();
    return {texSize.x * scale.x, texSize.y * scale.y};
}

/**
 * @brief Set the sprite size with a pair
 *
 * @param size First: width, Second: height
 */
void Sprite::SetSize(std::pair<float, float> size)
{
    SetSize(size.first, size.second);
}

/**
 * @brief Set the sprite size
 *
 * @param size_x Width
 * @param size_y Height
 */
void Sprite::SetSize(float size_x, float size_y)
{
    sf::Vector2u texSize = _texture.getSize();
    _sprite.setScale(size_x / texSize.x, size_y / texSize.y);
}

/**
 * @brief Multiply the sprite size
 *
 * @param multiplicator First: width multiplier, Second: height multiplier
 */
void Sprite::multiplySize(std::pair<float, float> multiplicator)
{
    sf::Vector2f scale = _sprite.getScale();
    _sprite.setScale(scale.x * multiplicator.first, scale.y * multiplicator.second);
}

/**
 * @brief Multiply the sprite size
 *
 * @param multiplicator_x Width multiplier
 * @param multiplicator_y Height multiplier
 */
void Sprite::multiplySize(float multiplicator_x, float multiplicator_y)
{
    sf::Vector2f scale = _sprite.getScale();
    _sprite.setScale(scale.x * multiplicator_x, scale.y * multiplicator_y);
}

/**
 * @brief Divide the sprite size
 *
 * @param divisor First: width divisor, Second: height divisor
 */
void Sprite::deviseSize(std::pair<float, float> divisor)
{
    if (divisor.first != 0 && divisor.second != 0)
    {
        sf::Vector2f scale = _sprite.getScale();
        _sprite.setScale(scale.x / divisor.first, scale.y / divisor.second);
    }
}

/**
 * @brief Divide the sprite size
 *
 * @param divisor_x Width divisor
 * @param divisor_y Height divisor
 */
void Sprite::deviseSize(float divisor_x, float divisor_y)
{
    if (divisor_x != 0 && divisor_y != 0)
    {
        sf::Vector2f scale = _sprite.getScale();
        _sprite.setScale(scale.x / divisor_x, scale.y / divisor_y);
    }
}

/**
 * @brief Return whether the sprite is visible
 *
 * @return true if visible
 * @return false if not visible
 */
bool Sprite::GetVisibility() const
{
    return _is_visible;
}

/**
 * @brief Set the sprite visibility
 *
 * @param visibility Visibility flag
 */
void Sprite::SetVisibility(bool visibility)
{
    _is_visible = visibility;
}

/**
 * @brief Make the sprite visible
 */
void Sprite::show()
{
    _is_visible = true;
}

/**
 * @brief Hide the sprite
 */
void Sprite::hide()
{
    _is_visible = false;
}
