#include "Attack.hpp"

#include <iostream>

/**
 * @brief Fire the attack based on fire rate
 *
 * @return true if the attack can be execute, false otherwise
 */
bool Attack::fire()
{
    Timestamp now = Clock::now();
    double elapsed = std::chrono::duration<double>(now - lastShot).count();

    if (elapsed >= _fireRate) {
        lastShot = now;
        std::cout << "Fire, damage: " << _damage << std::endl;
        return true;
    } else {
        _remaining = _fireRate - elapsed;
        std::cout << "Can't fire, time remaining: " << _remaining << " seconds" << std::endl;
        return false;
    }
    return false;
}

/**
 * @brief Get the Attack values
 *
 * @return std::pair<float, float> First: damage, Second: fire rate
 */
std::pair<float, float> Attack::GetAttack()
{
    return {_damage, _fireRate};
}

/**
 * @brief Set the Attack values, First: damage, Second: fire rate
 *
 * @param damage value to set damage
 * @param firerate value to set fire rate
 */
void Attack::SetAttack(int damage, float firerate)
{
    _damage = damage;
    _fireRate = firerate;
}

/**
 * @brief Add to _damage
 *
 * @param damage value to add
 */
void Attack::AddDamage(float damage)
{
    _damage += damage;
}

/**
 * @brief Subtract from _damage
 *
 * @param damage value to subtract
 */
void Attack::SubDamage(float damage)
{
    _damage -= damage;
}

/**
 * @brief Add to _fireRate
 *
 * @param firerate value to add
 */
void Attack::AddFireRate(float firerate)
{
    _fireRate += firerate;
}

/**
 * @brief Subtract from _fireRate
 *
 * @param firerate value to subtract
 */
void Attack::SubFireRate(float firerate)
{
    _fireRate -= firerate;
}
