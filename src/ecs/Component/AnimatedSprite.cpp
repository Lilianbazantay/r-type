#include "AnimatedSprite.hpp"

#include <stdexcept>

/**
 * @brief Construct a new AnimatedSprite object
 *
 * @param file_path Path to the texture file
 * @param size_x Initial width
 * @param size_y Initial height
 * @param number_of_sprite_x Number of frames in X
 * @param number_of_sprite_y Number of frames in Y
 */
AnimatedSprite::AnimatedSprite(const std::string& file_path, float size_x, float size_y, int number_of_sprite_x, int number_of_sprite_y)
    : _number_of_sprite_x(number_of_sprite_x), _number_of_sprite_y(number_of_sprite_y), _animation_rate(10.f)
{
    if (!_texture.loadFromFile(file_path))
        throw std::runtime_error("Failed to load texture: " + file_path);

    _sprite.setTexture(_texture);

    int frameWidth = _texture.getSize().x / _number_of_sprite_x;
    int frameHeight = _texture.getSize().y / _number_of_sprite_y;
    _currentRect = sf::IntRect(0, 0, frameWidth, frameHeight);
    _sprite.setTextureRect(_currentRect);

    _sprite.setScale(size_x / frameWidth, size_y / frameHeight);
    _type = ComponentType::ANIMATED_SPRITE;
}

/**
 * @brief Get the SFML sprite
 *
 * @return sf::Sprite& Reference to the sprite
 */
sf::Sprite& AnimatedSprite::GetSprite()
{
    return _sprite;
}

/**
 * @brief Set a new texture from file
 *
 * @param file_path Path to the new texture file
 */
void AnimatedSprite::SetTexture(const std::string& file_path)
{
    if (!_texture.loadFromFile(file_path))
        throw std::runtime_error("Failed to load texture: " + file_path);

    _sprite.setTexture(_texture, true);

    int frameWidth = _texture.getSize().x / _number_of_sprite_x;
    int frameHeight = _texture.getSize().y / _number_of_sprite_y;
    _currentRect = sf::IntRect(0, 0, frameWidth, frameHeight);
    _sprite.setTextureRect(_currentRect);
}

/**
 * @brief Return the sprite size
 *
 * @return std::pair<float, float> First: width, Second: height
 */
std::pair<float, float> AnimatedSprite::GetSize() const
{
    sf::Vector2f scale = _sprite.getScale();
    sf::Vector2i texSize = _currentRect.getSize();
    return {texSize.x * scale.x, texSize.y * scale.y};
}

/**
 * @brief Set the sprite size with a pair
 *
 * @param size First: width, Second: height
 */
void AnimatedSprite::SetSize(std::pair<float, float> size)
{
    SetSize(size.first, size.second);
}

/**
 * @brief Set the sprite size
 *
 * @param size_x Width
 * @param size_y Height
 */
void AnimatedSprite::SetSize(float size_x, float size_y)
{
    sf::Vector2u texSize(_currentRect.width, _currentRect.height);
    _sprite.setScale(size_x / texSize.x, size_y / texSize.y);
}

/**
 * @brief Multiply the sprite size
 *
 * @param multiplicator First: width multiplier, Second: height multiplier
 */
void AnimatedSprite::multiplySize(std::pair<float, float> multiplicator)
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
void AnimatedSprite::multiplySize(float multiplicator_x, float multiplicator_y)
{
    sf::Vector2f scale = _sprite.getScale();
    _sprite.setScale(scale.x * multiplicator_x, scale.y * multiplicator_y);
}

/**
 * @brief Divide the sprite size
 *
 * @param divisor First: width divisor, Second: height divisor
 */
void AnimatedSprite::deviseSize(std::pair<float, float> divisor)
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
void AnimatedSprite::deviseSize(float divisor_x, float divisor_y)
{
    if (divisor_x != 0 && divisor_y != 0)
    {
        sf::Vector2f scale = _sprite.getScale();
        _sprite.setScale(scale.x / divisor_x, scale.y / divisor_y);
    }
}

/**
 * @brief Return the number of frames in the animation
 *
 * @return std::pair<int, int> First: frames in X, Second: frames in Y
 */
std::pair<int, int> AnimatedSprite::GetNumberOfSprite() const
{
    return {_number_of_sprite_x, _number_of_sprite_y};
}

/**
 * @brief Set the number of frames in X and Y
 *
 * @param number_of_sprite First: frames in X, Second: frames in Y
 */
void AnimatedSprite::SetNumberOfSprite(std::pair<int, int> number_of_sprite)
{
    _number_of_sprite_x = number_of_sprite.first;
    _number_of_sprite_y = number_of_sprite.second;
}

/**
 * @brief Set the number of frames in X and Y
 *
 * @param number_of_sprite_x Frames in X
 * @param number_of_sprite_y Frames in Y
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
float AnimatedSprite::GetAnimationRate() const
{
    return _animation_rate;
}

/**
 * @brief Set the animation rate
 *
 * @param animation_rate New animation rate (frames per second)
 */
void AnimatedSprite::SetAnimationRate(float animation_rate)
{
    _animation_rate = animation_rate;
}

/**
 * @brief Return whether the sprite is visible
 *
 * @return true if visible
 * @return false if not visible
 */
bool AnimatedSprite::GetVisibility() const
{
    return _is_visible;
}

/**
 * @brief Set the sprite visibility
 *
 * @param visibility Visibility flag
 */
void AnimatedSprite::SetVisibility(bool visibility)
{
    _is_visible = visibility;
}

/**
 * @brief Make the sprite visible
 */
void AnimatedSprite::show()
{
    _is_visible = true;
}

/**
 * @brief Hide the sprite
 */
void AnimatedSprite::hide()
{
    _is_visible = false;
}

/**
 * @brief Update the animation frame based on elapsed time
 *
 * @param deltaTime Time elapsed since last update (in seconds)
 */
void AnimatedSprite::update(float deltaTime)
{
    if (!_is_visible || _animation_rate <= 0.f)
        return;

    _elapsed_time += deltaTime;
    float frameTime = 1.f / _animation_rate;

    while (_elapsed_time >= frameTime)
    {
        _elapsed_time -= frameTime;
        _current_frame_x++;

        if (_current_frame_x >= _number_of_sprite_x)
        {
            _current_frame_x = 0;
            _current_frame_y++;
            if (_current_frame_y >= _number_of_sprite_y)
                _current_frame_y = 0;
        }

        int frameWidth = _texture.getSize().x / _number_of_sprite_x;
        int frameHeight = _texture.getSize().y / _number_of_sprite_y;
        _currentRect.left = _current_frame_x * frameWidth;
        _currentRect.top = _current_frame_y * frameHeight;
        _currentRect.width = frameWidth;
        _currentRect.height = frameHeight;

        _sprite.setTextureRect(_currentRect);
    }
}
