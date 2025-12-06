#pragma once
#include "../IComponent.hpp"
#include <utility>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using Timestamp = std::chrono::time_point<Clock>;

/**
 * @class Attack
 * @brief Represents the Attack of a component
 *
 * @param _damage The Attack damage*
 * @param _fireRate The Attack fire rate (in seconds)
 * @param lastShot The last time the Attack was fired
 * @param _remaining The remaining time before the Attack can be fired again
 */
class Attack : public IComponent
{
    private:
        float _damage;
        float _fireRate;
        Timestamp lastShot;
        float _remaining;
    public:
        Attack(float damage, float firerate) : _damage(damage), _fireRate(firerate) {
            _type = ComponentType::ATTACK;
        };
        ~Attack() override = default;

        bool fire();

        // SET/GET
        std::pair<float, float> GetAttack();
        void SetAttack(int damage, float firerate);

        // ADD/SUB
        void AddDamage(float damage);
        void SubDamage(float damage);

        void AddFireRate(float firerate);
        void SubFireRate(float firerate);
};
