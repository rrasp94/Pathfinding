#pragma once

#include "SFML/System/Vector2.hpp"

namespace global {
    
    const sf::Vector2f MAP_SIZE = sf::Vector2f(1200.0f, 600.0f);
    const sf::Vector2f NODE_SIZE = sf::Vector2f(20.0f, 20.0f);

    const sf::Vector2u NODE_COUNT = sf::Vector2u(
        static_cast<unsigned int>(MAP_SIZE.x / NODE_SIZE.x),
        static_cast<unsigned int>(MAP_SIZE.y / NODE_SIZE.y)
    );
}
