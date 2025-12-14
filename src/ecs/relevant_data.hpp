
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include "Entity/IMediatorEntity.hpp"

typedef struct relevant_data_s {
    double runtime;
    sf::RenderWindow window;
    std::vector<std::unique_ptr<IMediatorEntity>> entityList;
    unsigned long bullet_count;
    unsigned long enemy_count;
} relevant_data_t;
