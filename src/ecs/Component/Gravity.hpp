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
    bool GetIsActivated();
    void SetisActivated(bool new_state);
    void Activate();
    void Unactivate();

    float GetAttractionStrength();
    void SetAttractionStrength(float new_strength);
    void MultiplyAttractionStrength(float multiplier);
    void DiviseAttractionStrength(float diviser);
};
