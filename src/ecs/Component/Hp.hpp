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
        int _maxHp = 1;
    public:
        Hp(int value) : _value(value), _maxHp(value) {
            if (_maxHp == 0)
                _maxHp++;
            _type = ComponentType::HP;
        };
        ~Hp() override = default;

        // SET/GET
        int GetHp();
        int GetHpPercent();
        void SetHp(int value);

        // ADD/SUB
        void AddHp();
        void AddHp(int value);
        void SubHp();
        void SubHp(int value);
};
