#pragma once
#include <utility>

/**
 * @class Position
 * @brief Represents the position of a component into a 2D space
 *
 * @param x The x coordinate with a float value
 * @param y The y coordinate with a float value
 */
class Position
{
    private:
        float x;
        float y;
    public:
        Position(float x = 0, float y = 0) : x(x), y(y) {};
        ~Position() = default;

        // SET/GET
        std::pair<float, float> GetPosition();
        void SetPosition(float newX, float newY);
        void SetPosition(std::pair<float, float> newPos);

        // MOVEMENT
        void moveUp();
        void moveUp(float distance);

        void moveDown();
        void moveDown(float distance);

        void moveLeft();
        void moveLeft(float distance);

        void moveRight();
        void moveRight(float distance);
};
