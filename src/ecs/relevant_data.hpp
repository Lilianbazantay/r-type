
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity/IMediatorEntity.hpp"

typedef struct relevant_data {
    float runtime;
    sf::RenderWindow window;
    std::vector<IMediatorEntity> entityList;
};