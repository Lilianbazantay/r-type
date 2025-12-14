#pragma once
#include "../IComponent.hpp"

/**
 * @class Velocity
 * @brief Represents the Velocity of a component
 *
 * @param _value The Velocity value
 */
class Velocity : public IComponent
{
    private:
        float _value;
    public:
        Velocity(float value) : _value(value) {
            _type = ComponentType::VELOCITY;
        };
        ~Velocity() override = default;

        // SET/GET
        float GetVelocity();
        void SetVelocity(float value);

        // ADD/SUB
        void AddVelocity(float value);
        void SubVelocity(float value);
};
