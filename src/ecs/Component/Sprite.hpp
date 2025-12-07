#pragma once

#include "../IComponent.hpp"

#include <string>

/**
 * @brief Class for a Sprite not animated like a bakground or an
 */
class Sprite : public IComponent
{
private:
    std::string _link;
    float _size_x;
    float _size_y;
    bool _is_visible = true;

public:
    Sprite(std::string link, float size_x, float size_y);
    ~Sprite() override = default;

    // SET / GET
    std::string GetLink();
    void SetLink(std::string link);

    std::pair<float, float> GetSize();
    void SetSize(std::pair<float, float> size);
    void SetSize(float size_x, float size_y);

    // OPERATOR
    void multiplySize(std::pair<float, float> multiplicator);
    void multiplySize(float multiplicatorX, float multiplicatorY);
    void deviseSize(std::pair<float, float> divisor);
    void deviseSize(float divisor_x, float divisor_y);

    // VISIBILITY
    bool GetVisibility();
    void SetVisibility(bool visibility);
    void show();
    void hide();
};
