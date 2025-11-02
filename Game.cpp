#include "Game.h"


Game::Game() 
	: window(sf::VideoMode(1500, 800), "Lunar Maze"),
	player("assets/player.png", {100, 100}, 150.f)
{
	map.loadFromFile("assets/map2.txt");

	player.scaleToTileSize(map.getTileSize());

	enemies.push_back(std::make_unique<Enemy>("assets/drone.png", sf::Vector2f(400, 300), 100.f));
	enemies.push_back(std::make_unique<Enemy>("assets/drone.png", sf::Vector2f(600, 200), 120.f));
	for (auto& enemy : enemies) enemy->scaleToTileSize(map.getTileSize());

	font.loadFromFile("assets/font.ttf");
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10, 10);

	titleText.setFont(font);
	titleText.setCharacterSize(60);
	titleText.setFillColor(sf::Color::Cyan);
	titleText.setString("Lunar Maze");
	titleText.setPosition(160, 150);

	instructionText.setFont(font);
	instructionText.setCharacterSize(24);
	instructionText.setFillColor(sf::Color(200, 200, 255));
	instructionText.setString("Collect all the crystals (*) while avoiding the drones!\nUse W/A/S/D to move your astronaut.");
	instructionText.setPosition(220, 300);


	statusText.setFont(font);
	statusText.setCharacterSize(40);
	statusText.setFillColor(sf::Color::Yellow);
	statusText.setPosition(200, 250);
}

void Game::run() {
	sf::Clock clock;
	while (window.isOpen()) {
		handleEvents();
		float dt = clock.restart().asSeconds();

		if (state == GameState::Playing)
			update(dt);

		draw();
	}
}
void Game::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Escape) {
				window.close();
			}

			if (state == GameState::Menu && event.key.code == sf::Keyboard::Space) {
				state = GameState::Playing;
			}
			if ((state == GameState::Win || state == GameState::GameOver) && event.key.code == sf::Keyboard::R) {
				resetGame();
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (state == GameState::Menu) {
					state = GameState::Playing;
				}
				else if (state == GameState::Win || state == GameState::GameOver) {
					resetGame();
				}
			}
		}
	}
}
void Game::update(float dt) {
	player.update(dt, &map);

	for (auto& enemy : enemies)
		enemy->update(dt, nullptr);

	for (auto& enemy : enemies) {
		if (player.getBounds().intersects(enemy->getBounds())) {
			state = GameState::GameOver;
			statusText.setString("Game Over! You were caught! Press R to restart.");
			statusText.setFillColor(sf::Color::Red);
			return;
		}
	}

	bool allCollected = true;
	for (const auto& tile : map.getTiles()) {
		if (tile.getType() == TileType::Crystal && !tile.isCollected()) {
			allCollected = false;
			break;
		}
	}
	if (allCollected) {
		state = GameState::Win;
		statusText.setString("You Win! All crystals collected! Press R to restart.");
		statusText.setFillColor(sf::Color::Green);
	}
	scoreText.setString("Score: " + std::to_string(map.getScore()));

}

void Game::draw() {
	window.clear();

	if (state == GameState::Menu) {
		window.draw(titleText);
		window.draw(instructionText);
	}
	else if (state == GameState::Playing) {
		map.draw(window);
		player.draw(window);
		for (auto& enemy : enemies)
			enemy->draw(window);
		window.draw(scoreText);
	}else if(state == GameState::GameOver || state == GameState::Win) {
		map.draw(window);
		player.draw(window);
		for (auto& enemy : enemies)
			enemy->draw(window);
		window.draw(scoreText);
		window.draw(statusText);
	}

	window.display();
}

void Game::resetGame()
{
	map.loadFromFile("assets/map2.txt");

	player.setPosition({ 100, 100 });
	player.scaleToTileSize(map.getTileSize());

	enemies.clear();
	enemies.push_back(std::make_unique<Enemy>("assets/drone.png", sf::Vector2f(400, 300), 100.f));
	enemies.push_back(std::make_unique<Enemy>("assets/drone.png", sf::Vector2f(600, 200), 120.f));
	for (auto& enemy : enemies) {
		enemy->scaleToTileSize(map.getTileSize());
		enemy->setPosition(enemy->getPosition());
	}

	state = GameState::Menu;
}
