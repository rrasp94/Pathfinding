#pragma once

#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/System/Vector3.hpp"

#include "GlobalVariables.hpp"


template<typename T>
inline bool Contains(const T& container, const typename T::key_type& object) {
	return container.find(object) != container.end();
}

inline uint32_t In2D(uint32_t x = 0, uint32_t y = 0, uint32_t sizeX = global::NODE_COUNT.x) {
	return x + y * sizeX;
}

// Converts sf::Color to normalized sf::Vector3f
inline sf::Vector3f ColorToVec3(sf::Color color) {
	return
	{
		color.r / 255.0f,
		color.g / 255.0f,
		color.b / 255.0f,
	};
}

// Converts normalized sf::Vector3f to sf::Color
inline sf::Color Vec3ToColor(sf::Vector3f color) {
	return
	{
		uint8_t(color.x * 255.0f),
		uint8_t(color.y * 255.0f),
		uint8_t(color.z * 255.0f),
		255,
	};
}
