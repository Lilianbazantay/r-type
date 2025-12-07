#pragma once

#include "../IComponent.hpp"

#include <string>

class Sprite : public IComponent
{
private:
    std::string _link;
    float _sizeX;
    float _sizeY;

public:
    Sprite(std::string link, float sizeX, float sizeY);
    ~Sprite() override = default;

    // SET / GET
    std::string GetLink();
    void SetLink(std::string link);

    std::pair<float, float> GetSize();
    void SetSize(std::pair<float, float> size);
    void SetSize(float sizeX, float sizeY);

    // OPERATOR
    void multiplySize(std::pair<float, float> multiplicator);
    void multiplySize(float multiplicatorX, float multiplicatorY);
    void deviseSize(std::pair<float, float> divisor);
    void deviseSize(float divisorX, float divisorY);
};
