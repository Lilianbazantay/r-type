#include "Hp.hpp"

/**
 * @brief Get the Hp object
 *
 * @return int The current Hp value
 */
int Hp::GetHp()
{
    return this->_number;
}

/**
 * @brief Set the Hp object
 *
 * @param number The new Hp value
 */
void Hp::SetHp(int number)
{
    this->_number = number;
}

/**
 * @brief Add 1 to the current Hp value
 */
void Hp::AddHp()
{
    this->_number += 1;
}

/**
 * @brief Add a specific number to the current Hp value
 *
 * @param number The number to add
 */
void Hp::AddHp(int number)
{
    this->_number += number;
}

/**
 * @brief Subtract 1 from the current Hp value
 */
void Hp::SubHp()
{
    this->_number -= 1;
}

/**
 * @brief Subtract a specific number from the current Hp value
 *
 * @param number The number to subtract
 */
void Hp::SubHp(int number)
{
    this->_number -= number;
}
