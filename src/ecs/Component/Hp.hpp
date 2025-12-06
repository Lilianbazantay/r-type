#pragma once
#include "../IComponent.hpp"

/**
 * @class Hp
 * @brief Represents the Hp of a component
 *
 * @param _value The Hp value
 */
class Hp : public IComponent
{
    private:
        int _value;
    public:
        Hp(int value) : _value(value) {
            _type = ComponentType::HP;
        };
        ~Hp() override = default;

        // SET/GET
        int GetHp();
        void SetHp(int value);

        // ADD/SUB
        void AddHp();
        void AddHp(int value);
        void SubHp();
        void SubHp(int value);
};
