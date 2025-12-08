#pragma once

#include "../IComponent.hpp"

class Gravity: public IComponent
{
private:
    bool _is_activated;
    float _attraction_strength;

public:
    Gravity(bool is_activated, float attraction_strength);
    ~Gravity() override = default;

    // GET / SET
    bool IsActivated();
    void SetActivationState(bool new_state);
    void Enable();
    void Disable();

    float GetAttractionStrength();
    void SetAttractionStrength(float new_strength);
    void MultiplyAttractionStrength(float multiplier);
    void DiviseAttractionStrength(float diviser);
};
