#include "AnimatedSprite.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
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
AnimatedSprite::AnimatedSprite(const std::string file_path, std::pair<float, float> scale, std::pair<float, float> size) {
    if (!_texture.loadFromFile(file_path))
        throw std::runtime_error("Failed to load texture: " + file_path);

    _sprite.setTexture(_texture);
    _currentRect = {0, 0, (int)size.first, (int)size.second};
    _sprite.setTextureRect(_currentRect);
    _scaleMultiplier = {scale.first, scale.second};
    _sprite.setScale(_scaleMultiplier);
    _type = ComponentType::ANIMATED_SPRITE;
    std::cout << "finished builder" << std::endl;
}

/**
 * @brief Set the animation rate
 *
 * @param animation_rate New animation rate (frames per second)
 */
void AnimatedSprite::setAnimationRate(float animation_rate) {
    _animationRate = animation_rate;
}

/**
 * @brief Set a new texture from file
 *
 * @param file_path Path to the new texture file
 */
void AnimatedSprite::setTexture(const std::string file_path) {
    if (!_texture.loadFromFile(file_path))
        throw std::runtime_error("Failed to load texture: " + file_path);
    _sprite.setTexture(_texture, true);
    //_sprite.setTextureRect(_currentRect);
}

/**
 * @brief Set the sprite size with a pair
 *
 * @param size First: width, Second: height
 */
void AnimatedSprite::setScale(std::pair<float, float> size) {
    _scaleMultiplier = {size.first, size.second};
    //_sprite.setScale(_scaleMultiplier);
}

/**
 * @brief Set the sprite size
 *
 * @param size_x Width
 * @param size_y Height
 */
void AnimatedSprite::setScale(float size_x, float size_y) {
    _scaleMultiplier = {size_x, size_y};
    //_sprite.setScale(_scaleMultiplier);
}


/**
 * @brief Return the animation rate
 *
 * @return float Current animation rate
 */
float AnimatedSprite::getAnimationRate() {
    return _animationRate;
}

/**
 * @brief Return the sprite size
 *
 * @return std::pair<unisgned int, unisgned int> First: width, Second: height
 */
std::pair<float, float> AnimatedSprite::getScale() {
    return {_scaleMultiplier.x, _scaleMultiplier.y};
}

/**
 * @brief Get the SFML sprite
 *
 * @return sf::Sprite& Reference to the sprite
 */
sf::Sprite& AnimatedSprite::getSprite() {
    update();
    return _sprite;
}

/**
 * @brief Return the sprite size
 *
 * @return std::pair<float, float> First: width, Second: height
 */
std::pair<float, float> AnimatedSprite::getSize() const
{
    sf::Vector2f scale = _sprite.getScale();
    sf::IntRect texSize = _sprite.getTextureRect();
    return {texSize.width * scale.x, texSize.height * scale.y};
}

/**
 * @brief Update the animation frame based on elapsed time
 */
void AnimatedSprite::update() {
    if (_animationArray.empty() || !_visible) {
        std::cout << "empty" << std::endl;
        return;
    }
    if (_clock.getElapsedTime().asSeconds() > _animationRate) {
        _clock.restart();
        _actualLength++;
        if (_actualLength == _actualAnimation.length) {
            _locked = false;
            if (_actualAnimation.loop) {
                changeAnimation(_actualAnimation.type, false);
            } else {
                changeAnimation(IDLE, false);
            }
        } else {
            _currentRect.left += _actualAnimation.offset.x;
            _currentRect.top += _actualAnimation.offset.y;
            _sprite.setTextureRect(_currentRect);
        }
    }
}

void AnimatedSprite::changeAnimation(const ANIMATION_TYPE type, bool locked) {
    if (!AnimationExists(type))
        return;
    if (_locked) {
        _lockedAnimation = type;
        return;
    }
    if (_lockedAnimation == NOTHING)
        _lockedAnimation = type;
    _actualAnimation.type = NOTHING;
    for (size_t i = 0; i < _animationArray.size(); i++)
        if (_animationArray[i].type == _lockedAnimation) {
            _actualAnimation = _animationArray[i];
            break;
        }
    resetAnimation();
    _locked = locked;
    _lockedAnimation = NOTHING;
}

void AnimatedSprite::resetAnimation() {
    if (_actualAnimation.type == NOTHING)
        return;
    _currentRect.left = _actualAnimation.frameOrigin.x;
    _currentRect.top = _actualAnimation.frameOrigin.y;
    _currentRect = {(int)_actualAnimation.frameOrigin.x, (int)_actualAnimation.frameOrigin.y,
        (int)_actualAnimation.frameSize.x, (int)_actualAnimation.frameSize.y};
    _sprite.setTextureRect(_currentRect);
    _actualLength = 0;
}

void AnimatedSprite::addAnimation(std::pair<float, float> frameSize, std::pair<float, float> origin, std::pair<float, float> movement, ANIMATION_TYPE type, bool loop, int length) {
    Animation_t newAnimation;
    newAnimation.frameSize = {frameSize.first, frameSize.second};
    newAnimation.frameOrigin = {origin.first, origin.second};
    newAnimation.framePosition = {origin.first, origin.second};
    newAnimation.offset = {movement.first, movement.second};
    newAnimation.type = type;
    newAnimation.loop = loop;
    newAnimation.length = length;
    _animationArray.push_back(newAnimation);
}


bool AnimatedSprite::AnimationExists(const ANIMATION_TYPE type) {
    for (size_t i = 0; i < _animationArray.size(); i++)
        if (_animationArray[i].type == type)
            return true;
    return false;
}

/**
 * @brief Return whether the sprite is visible
 *
 * @return true if visible
 * @return false if not visible
 */
bool AnimatedSprite::getVisibility() {
    return _visible;
}


/**
 * @brief Make the sprite visible
 */
void AnimatedSprite::show() {
    _visible = true;
}

/**
 * @brief Hide the sprite
 */
void AnimatedSprite::hide() {
    _visible = false;
}
