#include "Direction.hpp"
#include <cmath>

constexpr float DEG_TO_RAD = 3.14159265f / 180.f;
constexpr float RAD_TO_DEG = 180.f / 3.14159265f;

/**
 * @brief Construct a Direction component from a vector (x, y).
 * @param x X component of the vector.
 * @param y Y component of the vector.
 *
 * Initializes the angle in degrees based on the vector.
 */
Direction::Direction(float x, float y)
    : x(x), y(y)
{
    _type = ComponentType::DIRECTION;
    updateAngleFromVector();
}

/**
 * @brief Construct a Direction component from an angle in degrees.
 * @param angleDeg Angle in degrees.
 *
 * Initializes the vector based on the angle.
 */
Direction::Direction(float angleDeg) : angle(angleDeg)
{
    _type = ComponentType::DIRECTION;
    updateVectorFromAngle();
}

/**
 * @brief Get the direction as a 2D vector.
 * @return std::pair<float, float> Vector representing the direction.
 */
std::pair<float, float> Direction::GetDirection() const
{
    return {x, y};
}

/**
 * @brief Set the direction using x and y components.
 * @param newX New x component.
 * @param newY New y component.
 *
 * Updates the angle in degrees based on the new vector.
 */
void Direction::SetDirection(float newX, float newY)
{
    x = newX;
    y = newY;
    updateAngleFromVector();
}

/**
 * @brief Set the direction using a vector pair.
 * @param vec Pair representing the new direction (x, y).
 *
 * Updates the angle in degrees based on the new vector.
 */
void Direction::SetDirection(const std::pair<float, float>& vec)
{
    x = vec.first;
    y = vec.second;
    updateAngleFromVector();
}

/**
 * @brief Get the angle in degrees.
 * @return float Angle in degrees.
 */
float Direction::getAngle() const
{
    return angle;
}

/**
 * @brief Set the angle in degrees.
 * @param deg New angle in degrees.
 *
 * Updates the vector based on the new angle.
 */
void Direction::setAngleDegrees(float deg)
{
    angle = deg;
    updateVectorFromAngle();
}

/**
 * @brief Normalize the direction vector to have a magnitude of 1.
 *
 * Updates the angle in degrees to stay in sync.
 * Does nothing if the vector has zero length.
 */
void Direction::normalize()
{
    float norm = std::sqrt(x * x + y * y);
    if (norm != 0.f)
    {
        x /= norm;
        y /= norm;
        updateAngleFromVector();
    }
}

/**
 * @brief Rotate the direction by a given angle in degrees.
 * @param angleDeg Angle in degrees to rotate the direction.
 *
 * Updates both the vector and the angle.
 */
void Direction::rotateDegrees(float angleDeg)
{
    angle += angleDeg;
    updateVectorFromAngle();
}

/**
 * @brief Update the vector (x, y) based on the current angle in degrees.
 *
 * Converts the angle in degrees to radians for std::cos and std::sin.
 */
void Direction::updateVectorFromAngle()
{
    float rad = angle * DEG_TO_RAD;
    x = std::cos(rad);
    y = std::sin(rad);
}

/**
 * @brief Update the angle in degrees based on the current vector (x, y).
 *
 * Uses std::atan2 to calculate the angle and converts it from radians to degrees.
 */
void Direction::updateAngleFromVector()
{
    angle = std::atan2(y, x) * RAD_TO_DEG;
}
