#include "Game.hpp"
#include "utilities/Functions.hpp"

#include <algorithm>

Game::Game()

	: m_window(sf::VideoMode({
		  static_cast<unsigned int>(global::NODE_COUNT.x * global::NODE_SIZE.x),
		  static_cast<unsigned int>(global::NODE_COUNT.y * global::NODE_SIZE.y)
		}),
		"SFML Pathfinding")
{
	init();
}

void Game::init() {
	// Attach renderer to window
	Renderer.Init(m_window);

	// Set a view covering the whole grid (fixes hover/mouse offset on resize)
	sf::Vector2f size(
		static_cast<float>(global::NODE_COUNT.x * global::NODE_SIZE.x),
		static_cast<float>(global::NODE_COUNT.y * global::NODE_SIZE.y)
	);
	sf::Vector2f center = size * 0.5f;
	sf::View view(center, size);
	m_window.setView(view);

	// Create data
	m_nodes.Create();
	m_quads.Create(/*createGrid=*/true);
	m_quads.SetPosition({ 0.f, 0.f });

	// Start/finish roughly left & right center
	m_startCell = { 2u, global::NODE_COUNT.y / 2u };
	m_finishCell = { std::max(1u, global::NODE_COUNT.x - 3u), global::NODE_COUNT.y / 2u };

	// Ensure walkable and visually mark
	m_nodes(m_startCell).Walkable = true;
	m_nodes(m_finishCell).Walkable = true;

	m_quads.Set(m_startCell, sf::Color(0, 200, 0, 255)); // green
	m_quads.Set(m_finishCell, sf::Color(200, 0, 0, 255)); // red
}

void Game::Run() {
	m_window.setFramerateLimit(120);
	while (m_running && m_window.isOpen()) {
		processEvents();
		update(1.f / 120.f);
		render();
	}
}

void Game::processEvents() {
	sf::Event ev{};
	while (m_window.pollEvent(ev)) {
		switch (ev.type) {
		case sf::Event::Closed:
			m_running = false; m_window.close(); break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Key::Escape) { m_running = false; m_window.close(); }
			if (ev.key.code == sf::Keyboard::Key::R) { // full reset
				m_animating = false; m_foundOrDone = false;
				m_nodes.Destroy();
				m_nodes.Create();
				m_quads.Reset();
				m_nodes(m_startCell).Walkable = true;
				m_nodes(m_finishCell).Walkable = true;
				m_quads.Set(m_startCell, sf::Color(0, 200, 0, 255));
				m_quads.Set(m_finishCell, sf::Color(200, 0, 0, 255));
			}
			if (ev.key.code == sf::Keyboard::Key::Space) {
				restartPathfinding();
				m_animating = true;
				m_foundOrDone = false;
				m_animClock.restart();
			}
			break;
		default: break;
		}
	}

	if (!m_animating && !m_foundOrDone) {
		// mouse painting (continuous while pressed)
		if (auto cell = mouseToCell()) {
			// un-hover previous
			if (m_hoverCell && *m_hoverCell != *cell) {
				const auto& last = *m_hoverCell;
				// only clear hover if not wall, not path, not endpoints
				auto& n = m_nodes(last);
				if (n.Walkable && !n.Path && last != m_startCell && last != m_finishCell)
					m_quads.Set(last, sf::Color(0, 0, 0, 0));
			}
			m_hoverCell = *cell;

			// set hover color (light gray) if walkable and not endpoints
			auto& hn = m_nodes(*cell);
			if (hn.Walkable && *cell != m_startCell && *cell != m_finishCell && !hn.Path)
				m_quads.Set(*cell, sf::Color(160, 160, 160, 160));

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				paintCell(*cell, /*wall=*/true);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
				paintCell(*cell, /*wall=*/false);
			}
		}
	}
}

void Game::update(float /*dt*/) {
	if (m_animating && !m_foundOrDone) {
		if (m_animClock.getElapsedTime().asSeconds() >= m_stepInterval) {
			m_animClock.restart();
			stepPathfinding();
		}
	}

	// extra step: draw path nodes gradually once found
	if (!m_finalPath.empty() && m_pathDrawIndex < m_finalPath.size()) {
		if (m_animClock.getElapsedTime().asSeconds() >= m_stepInterval) {
			m_animClock.restart();
			auto cell = m_finalPath[m_pathDrawIndex++];
			if (cell != m_startCell && cell != m_finishCell) {
				m_quads.Set(cell, sf::Color(0, 0, 255)); // blue
			}
			if (m_pathDrawIndex >= m_finalPath.size()) {
				m_finalPath.clear();
				m_animating = false;
				m_foundOrDone = true;
			}
		}
	}
}

void Game::render() {
	// white background (Renderer::Clear uses white, see note below)
	Renderer.Clear();

	// grid, walls, exploration/colors
	m_quads.Render();

	// force start/finish colors on top every frame
	m_quads.Set(m_startCell, sf::Color(0, 200, 0, 255));
	m_quads.Set(m_finishCell, sf::Color(200, 0, 0, 255));

	Renderer.Flush();
}

std::optional<sf::Vector2u> Game::mouseToCell() const {
	auto mousePosWindow = sf::Mouse::getPosition(m_window);
	auto mousePosWorld = m_window.mapPixelToCoords(mousePosWindow);

	if (mousePosWorld.x < 0 || mousePosWorld.y < 0) return std::nullopt;

	const auto cellX = static_cast<unsigned>(mousePosWorld.x / global::NODE_SIZE.x);
	const auto cellY = static_cast<unsigned>(mousePosWorld.y / global::NODE_SIZE.y);

	if (cellX >= global::NODE_COUNT.x || cellY >= global::NODE_COUNT.y)
		return std::nullopt;

	return sf::Vector2u(cellX, cellY);
}

void Game::paintCell(sf::Vector2u cell, bool wall) {
	// don't allow painting over start/finish
	if (cell == m_startCell || cell == m_finishCell) return;
	auto& n = m_nodes(cell);
	n.Walkable = !wall;
	n.Path = false;
	m_quads.Set(cell, wall ? sf::Color::Black : sf::Color(0, 0, 0, 0));
}

void Game::restartPathfinding() {
	// Clear prior path/exploration colors but keep walls
	for (auto& n : m_nodes.GetNodes()) {
		n.Path = false;
		if (n.Walkable) {
			m_quads.Set({ n.x, n.y }, sf::Color(0, 0, 0, 0));
		}
	}
	// Ensure endpoints are walkable and colored
	m_nodes(m_startCell).Walkable = true;
	m_nodes(m_finishCell).Walkable = true;
	m_quads.Set(m_startCell, sf::Color(0, 200, 0, 255));
	m_quads.Set(m_finishCell, sf::Color(200, 0, 0, 255));

	m_pathfinder.Restart();
}

void Game::stepPathfinding() {
	auto& nodes = m_nodes.GetNodes();
	auto& start = m_nodes(m_startCell);
	auto& goal = m_nodes(m_finishCell);

	auto [ok, state] = m_pathfinder.FindPath(nodes, start, goal, &m_quads);

	if (!ok) {
		m_animating = false;
		m_foundOrDone = true;
		return;
	}

	if (state == AnimatedPathfindState::Found) {
    // Clear previous final path
    m_finalPath.clear();

    // Trace path from goal to start using Parent
    Node* current = &goal;
    while (current != nullptr && current != &start) {
        m_finalPath.push_back(sf::Vector2u(current->x, current->y));
        current = current->Parent; 
    }

    // Add start cell at the end
    m_finalPath.push_back(sf::Vector2u(start.x, start.y));

    // Reverse to get start - goal order
    std::reverse(m_finalPath.begin(), m_finalPath.end());

    m_pathDrawIndex = 0;   // start animating the path
    return;
}

	if (state == AnimatedPathfindState::NotFound) {
		m_animating = false;
		m_foundOrDone = true;
		return;
	}

	// still exploring – color frontier / visited
	colorCurrentPath();
}

void Game::colorCurrentPath() {
	for (auto& n : m_nodes.GetNodes()) {
		if (n.Path && n.Walkable) {
			m_quads.Set({ n.x, n.y }, sf::Color(50, 140, 255, 230)); // path = blue
		}
	}
}