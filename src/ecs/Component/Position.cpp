#include "Position.hpp"

/**
 * @brief Return the current position as a pair float
 *
 * @return std::pair<float, float>
 */
std::pair<float, float> Position::GetPosition()
{
    return std::make_pair(x, y);
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
    x = newX;
    y = newY;
}

/**
 * @brief Set a new position
 *
 * @param newPos new position as a pair of float
 */
void Position::SetPosition(std::pair<float, float> newPos)
{
    x = newPos.first;
    y = newPos.second;
}

/**
 * @brief Move the position up by 1 unit
 *
 * @return void
 */
void Position::moveUp()
{
    y += 1;
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
    y += distance;
}

/**
 * @brief Move the position down by 1 unit
 *
 * @return void
 */
void Position::moveDown()
{
    y -= 1;
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
    y -= distance;
}

/**
 * @brief Move the position left by 1 unit
 *
 * @return void
 */
void Position::moveLeft()
{
    x -= 1;
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
    x -= distance;
}

/**
 * @brief Move the position right by 1 unit
 *
 * @return void
 */
void Position::moveRight()
{
    x += 1;
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
    x += distance;
}
