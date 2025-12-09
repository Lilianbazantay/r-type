#include "Position.hpp"

/**
 * @brief Return the current position as a pair float
 *
 * @return std::pair<float, float>
 */
std::pair<float, float> Position::GetPosition()
{
    return std::make_pair(_x, _y);
}

/**
 * @brief Set a new position
 *
 * @param newX new X coordinate with a float value
 * @param newY new Y coordinate with a float value
 *
 * @return void
 */
void Position::SetPosition(float newX, float newY)
{
    _x = newX;
    _y = newY;
}

/**
 * @brief Set a new position
 *
 * @param newPos new position as a pair of float
 */
void Position::SetPosition(std::pair<float, float> newPos)
{
    _x = newPos.first;
    _y = newPos.second;
}

/**
 * @brief Move the position up by 1 unit
 *
 * @return void
 */
void Position::moveUp()
{
    _y += 1;
}

/**
 * @brief Move the position up by "distance" unit
 *
 * @param distance distance to move
 *
 * @return void
 */
void Position::moveUp(float distance)
{
    _y += distance;
}

/**
 * @brief Move the position down by 1 unit
 *
 * @return void
 */
void Position::moveDown()
{
    _y -= 1;
}

/**
 * @brief Move the position down by "distance" unit
 *
 * @param distance distance to move
 *
 * @return void
 */
void Position::moveDown(float distance)
{
    _y -= distance;
}

/**
 * @brief Move the position left by 1 unit
 *
 * @return void
 */
void Position::moveLeft()
{
    _x -= 1;
}

/**
 * @brief Move the position left by "distance" unit
 *
 * @param distance distance to move
 *
 * @return void
 */
void Position::moveLeft(float distance)
{
    _x -= distance;
}

/**
 * @brief Move the position right by 1 unit
 *
 * @return void
 */
void Position::moveRight()
{
    _x += 1;
}

/**
 * @brief Move the position right by "distance" unit
 *
 * @param distance distance to move
 *
 * @return void
 */
void Position::moveRight(float distance)
{
    _x += distance;
}
