
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include "Entity/IMediatorEntity.hpp"

typedef struct relevant_data_s {
    double runtime;
    sf::RenderWindow window;
    std::vector<std::unique_ptr<IMediatorEntity>> entityList;
} relevant_data_t;