
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity/IMediatorEntity.hpp"

typedef struct relevant_data_s {
    float runtime;
    sf::RenderWindow window;
    std::vector<IMediatorEntity> entityList;
} relevant_data_t;