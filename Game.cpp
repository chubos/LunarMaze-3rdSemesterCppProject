#include "Game.h"

Game::Game() 
	: window(sf::VideoMode(800, 600), "Lunar Maze"),
	player("assets/player.png", {100, 100}, 150.f)
{
	enemies.push_back(std::make_unique<Enemy>("assets/drone.png", sf::Vector2f(400, 300), 100.f));
	enemies.push_back(std::make_unique<Enemy>("assets/drone.png", sf::Vector2f(600, 200), 120.f));
}

void Game::run() {
	sf::Clock clock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		float dt = clock.restart().asSeconds();

		player.update(dt);
		for (auto& e : enemies)
			e->update(dt);

		window.clear();
		map.draw(window);
		player.draw(window);
		for (auto& e : enemies)
			e->draw(window);
		window.display();
	}
}