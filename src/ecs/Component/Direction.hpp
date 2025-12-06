#pragma once

#include <utility>
#include <cmath>
#include "../IComponent.hpp"

/**
 * @class Direction
 * @brief Represents a 2D direction using both a vector and an angle in degrees.
 *
 * The angle is stored in degrees. Vector and angle are synchronysed.
 */
class Direction : public IComponent
{
private:
    float x;
    float y;
    float angle;

    void updateVectorFromAngle();
    void updateAngleFromVector();

public:
    // Constructors
    Direction(float x = 0.f, float y = 0.f);
    Direction(float angleDeg);

    ~Direction() override = default;

    // GET / SET VECTOR
    std::pair<float, float> GetDirection() const;
    void SetDirection(float newX, float newY);
    void SetDirection(const std::pair<float, float>& vec);

    // GET / SET ANGLE
    float getAngle() const;
    void setAngleDegrees(float deg);

    // NORMALIZE VECTOR
    void normalize();

    // ROTATION (degrees)
    void rotateDegrees(float angleDeg);
};
