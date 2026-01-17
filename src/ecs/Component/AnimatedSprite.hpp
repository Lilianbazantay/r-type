#pragma once

#include "../IComponent.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

typedef enum {
    NOTHING = -1,
    IDLE = 0,
    UP = 1,
    DOWN = 2,
    SHOOT = 3,
    DEATH = 4
} ANIMATION_TYPE;

typedef struct Animation_s {
    bool loop = false;
    ANIMATION_TYPE type = NOTHING;
    int length = 0;
    sf::Vector2f frameSize = {0, 0};
    sf::Vector2f frameOrigin = {0, 0};
    sf::Vector2f framePosition = {0, 0};
    sf::Vector2f offset = {0, 0};
} Animation_t;

class AnimatedSprite : public IComponent
{
    private:
        sf::Texture _texture;
        sf::Sprite _sprite;
        sf::IntRect _currentRect = {0, 0, 0, 0};

        sf::Clock _clock;
        bool _visible = true;
        bool _locked = false;
        float _animationRate = 0.3f;
        int _actualLength = 0;

        ANIMATION_TYPE _lockedAnimation = NOTHING;

        std::vector<Animation_t> _animationArray = {};
        Animation_t _actualAnimation;
        sf::Vector2f _scaleMultiplier = {1, 1};
    public:
        AnimatedSprite(const std::string, std::pair<float, float>, std::pair<float, float>);
        ~AnimatedSprite() = default;

        void setScale(float, float);
        void setScale(std::pair<float, float>);
        void setAnimationRate(float);
        void setTexture(const std::string);

        void hide();
        void show();
        void update();
        void changeAnimation(const ANIMATION_TYPE, bool);
        void resetAnimation();
        void addAnimation(std::pair<float, float>, std::pair<float, float>, std::pair<float, float>, ANIMATION_TYPE, bool, int);
        bool AnimationExists(const ANIMATION_TYPE);

        float getAnimationRate();
        bool getVisibility();
        std::pair<float, float> getScale();
        std::pair<float, float> getSize() const;
        sf::Sprite &getSprite();
};
