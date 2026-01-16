#include "Hp.hpp"

/**
 * @brief Get the Hp object
 *
 * @return int The current Hp value
 */
int Hp::GetHp()
{
    return this->_value;
}

/**
 * @brief Get the Hp object
 *
 * @return int The current Hp value in percent
 */
int Hp::GetHpPercent()
{
    return (_value / _maxHp) * 100;
}

/**
 * @brief Set the Hp object
 *
 * @param value The new Hp value
 */
void Hp::SetHp(int value)
{
    this->_value = value;
}

/**
 * @brief Add 1 to the current Hp value
 */
void Hp::AddHp()
{
    this->_value += 1;
}

/**
 * @brief Add a specific value to the current Hp value
 *
 * @param value The value to add
 */
void Hp::AddHp(int value)
{
    this->_value += value;
}

/**
 * @brief Subtract 1 from the current Hp value
 */
void Hp::SubHp()
{
    this->_value -= 1;
}

/**
 * @brief Subtract a specific value from the current Hp value
 *
 * @param value The value to subtract
 */
void Hp::SubHp(int value)
{
    this->_value -= value;
}
