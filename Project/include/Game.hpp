#pragma once

#include <optional>
#include <SFML/Graphics.hpp>


#include "core/Renderer.hpp"

#include "pathfinding/NodeMap.hpp"
#include "pathfinding/Pathfinder.hpp"
#include "QuadMap.hpp"

class Game {
public:

	Game();

	void Run();

private:
	// lifecycle
	void init();
	void processEvents();
	void update(float dt);
	void render();

	// input helpers
	std::optional<sf::Vector2u> mouseToCell() const;
	void paintCell(sf::Vector2u cell, bool wall);

	// pathfinding control
	void restartPathfinding();
	void stepPathfinding();
	void colorCurrentPath();

	// state
	sf::RenderWindow m_window;
	priv::Renderer   m_renderer;
	QuadMap          m_quads;
	NodeMap          m_nodes;
	AnimatedPathfinder m_pathfinder;

	// start/finish cells
	sf::Vector2u     m_startCell;
	sf::Vector2u     m_finishCell;

	// hover state so we can un-hover old cell
	std::optional<sf::Vector2u> m_hoverCell;

	// pacing for animation (no slider; fixed tempo)
	sf::Clock   m_animClock;
	float       m_stepInterval = 0.016f; // ~60 steps/sec

	// flags
	bool m_running = true;
	bool m_animating = false;
	bool m_foundOrDone = false;
	bool m_paused = false;

	std::vector<sf::Vector2u> m_finalPath;
	size_t m_pathDrawIndex = 0;
};