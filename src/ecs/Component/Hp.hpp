#pragma once
#include "../IComponent.hpp"

/**
 * @class Hp
 * @brief Represents the Hp of a component
 *
 * @param _number The Hp value
 */
class Hp : public IComponent
{
    private:
        int _number;
    public:
        Hp(int number) : _number(number) {
            _type = ComponentType::HP;
        };
        ~Hp() override = default;

        // SET/GET
        int GetHp();
        void SetHp(int number);

        // ADD/SUB
        void AddHp();
        void AddHp(int number);
        void SubHp();
        void SubHp(int number);
};
