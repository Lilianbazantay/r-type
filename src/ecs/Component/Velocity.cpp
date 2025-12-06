#include "Velocity.hpp"

/**
 * @brief Get the Velocity object
 *
 * @return float The current Velocity value
 */
float Velocity::GetVelocity()
{
    return this->_value;
}

/**
 * @brief Set the Velocity object
 *
 * @param value The new Velocity value
 */
void Velocity::SetVelocity(float value)
{
    this->_value = value;
}

/**
 * @brief Add a specific value to the current Velocity value
 *
 * @param value The value to add
 */
void Velocity::AddVelocity(float value)
{
    this->_value += value;
}

/**
 * @brief Subtract a specific value from the current Velocity value
 *
 * @param value The value to subtract
 */
void Velocity::SubVelocity(float value)
{
    this->_value -= value;
}
