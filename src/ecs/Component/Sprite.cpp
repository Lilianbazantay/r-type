#include "Sprite.hpp"

/**
 * @brief Construct a new Sprite:: Sprite object
 *
 * @param link is the link of the sprite
 * @param sizeX set the size x
 * @param sizeY set the size y
 */
Sprite::Sprite(std::string link, float sizeX, float sizeY): _link(link), _sizeX(sizeX), _sizeY(sizeY)
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
 * @return std::pair<float, float> First : sizeX, Second: sizeY
 */
std::pair<float, float> Sprite::GetSize()
{
    return {_sizeX, _sizeY};
}

/**
 * @brief Set the sprite size with a pair
 *
 * @param size First: sizeX, Second: sizeY
 */
void Sprite::SetSize(std::pair<float, float> size)
{
    _sizeX = size.first;
    _sizeY = size.second;
}

/**
 * @brief Set the Sprite size
 *
 * @param sizeX Set _sizeX
 * @param sizeY Set _sizeY
 */
void Sprite::SetSize(float sizeX, float sizeY)
{
    _sizeX = sizeX;
    _sizeY = sizeY;
}

/**
 * @brief Multiply the _sizeX and _sizeY of the sprite
 *
 * @param multiplicator value the size is multiply by. First: sizeX, Second: sizeY
 */
void Sprite::multiplySize(std::pair<float, float> multiplicator)
{
    _sizeX *= multiplicator.first;
    _sizeY *= multiplicator.second;
}

/**
 * @brief Multiply the _sizeX and _sizeY of the sprite
 *
 * @param multiplicatorX value _sizeX is multiply by
 * @param multiplicatorY value _sizeY is multiply by
 */
void Sprite::multiplySize(float multiplicatorX, float multiplicatorY)
{
    _sizeX *= multiplicatorX;
    _sizeY *= multiplicatorY;
}

/**
 * @brief Divise the _sizeX and _sizeY of the sprite
 *
 * @param divisor value the size is devise by. First: sizeX, Second: sizeY
 */
void Sprite::deviseSize(std::pair<float, float> divisor)
{
    if (divisor.first != 0 && divisor.second != 0) {
        _sizeX /= divisor.first;
        _sizeY /= divisor.second;
    }
}

/**
 * @brief Divise the _sizeX and _sizeY of the sprite
 *
 * @param divisorX value _sizeX is divise by
 * @param divisorY value _sizeY is divise by
 */
void Sprite::deviseSize(float divisorX, float divisorY)
{
    if (divisorX != 0 && divisorY != 0) {
        _sizeX /= divisorX;
        _sizeY /= divisorY;
    }
}
