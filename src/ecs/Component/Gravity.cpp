#include "Gravity.hpp"

/**
 * @brief Construct a new Gravity::Gravity object
 *
 * @param is_activated Set if the Entity has a gravity or not
 * @param attraction_strength Set the strength of the gravity
 */
Gravity::Gravity(bool is_activated, float attraction_strength): _is_activated(is_activated), _attraction_strength(attraction_strength)
{
    _type = ComponentType::GRAVITY;
}

/**
 * @brief Return if the gravity is activated
 *
 * @return true The gravity is enable
 * @return false The gravity is disable
 */
bool Gravity::GetActivationState()
{
    return _is_activated;
}

/**
 * @brief Set if the gravity is enable
 *
 * @param new_state Set _is_activated
 */
void Gravity::SetActivationState(bool new_state)
{
    _is_activated = new_state;
}

/**
 * @brief Activate the gravity
 */
void Gravity::Enable()
{
    _is_activated = true;
}

/**
 * @brief Disable the gravity
 */
void Gravity::Disable()
{
    _is_activated = false;
}

/**
 * @brief Return the attraction strength
 *
 * @return float Attraction strength
 */
float Gravity::GetAttractionStrength()
{
    return _attraction_strength;
}

/**
 * @brief Set the attraction strength
 *
 * @param new_strength New Attraction strength
 */
void Gravity::SetAttractionStrength(float new_strength)
{
    _attraction_strength = new_strength;
}

/**
 * @brief Multiply the attraction strength
 *
 * @param multiplier
 */
void Gravity::MultiplyAttractionStrength(float multiplier)
{
    _attraction_strength *= multiplier;
}

/**
 * @brief Divise the attraction strength
 *
 * @param diviser If = 0 do nothing
 */
void Gravity::DiviseAttractionStrength(float diviser)
{
    if (diviser != 0)
        _attraction_strength /= diviser;
}
