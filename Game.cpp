#include "Game.h"
#include "Random.h"
#include "Chaser.h"
#include "Learner.h"
#include <iostream>


Game::Game() 
	: window(sf::VideoMode(1700, 900), "Lunar Maze"),
	player("assets/player3.png", {100, 100}, 150.f)
{
	map.loadFromFile("assets/map2.txt");

	player.scaleToTileSize(map.getTileSize());

	enemies.push_back(std::make_unique<Random>("assets/ghost1.png", sf::Vector2f(300, 300), 100.f));
	enemies.push_back(std::make_unique<Chaser>("assets/ghost2.png", sf::Vector2f(300, 300), 120.f));
	enemies.push_back(std::make_unique<Learner>("assets/ghost3.png", sf::Vector2f(400, 500), 80.f));
	enemies.push_back(std::make_unique<Learner>("assets/ghost4.png", sf::Vector2f(600, 600), 80.f));
	for (auto& enemy : enemies) enemy->scaleToTileSize(map.getTileSize());

	font.loadFromFile("assets/font.ttf");
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10, 10);

	titleText.setFont(font);
	titleText.setCharacterSize(120);
	titleText.setFillColor(sf::Color::Cyan);
	titleText.setString("Lunar Maze");
	titleText.setPosition(300, 150);

	instructionText.setFont(font);
	instructionText.setCharacterSize(30);
	instructionText.setFillColor(sf::Color(200, 200, 255));
	instructionText.setString("Collect all the crystals (*) while avoiding the drones!\nUse W/A/S/D to move your astronaut.");
	instructionText.setPosition(260, 310);


	statusText.setFont(font);
	statusText.setCharacterSize(40);
	statusText.setFillColor(sf::Color::Yellow);
	statusText.setPosition(200, 250);

	if (!backgroundTexture.loadFromFile("assets/background5.png"))
		std::cerr << "Nie mo¿na za³adowaæ t³a!\n";
	background.setTexture(backgroundTexture);
	background.setScale(
		window.getSize().x / background.getGlobalBounds().width,
		window.getSize().y / background.getGlobalBounds().height
	);

	if(!music.openFromFile("assets/music.ogg"))
		std::cerr << "Failed to load background music\n";
	else {
		music.setLoop(true);
		music.setVolume(40);
		music.play();
	}

	// Load short sound buffers and attach them to sounds
	if (!collectBuffer.loadFromFile("assets/collect.wav"))
		std::cerr << "Failed to load collect.wav\n";
	else {
		collectSound.setBuffer(collectBuffer);
		collectSound.setVolume(60.f);
	}

	if (!loseBuffer.loadFromFile("assets/lose.wav"))
		std::cerr << "Failed to load lose.wav\n";
	else {
		loseSound.setBuffer(loseBuffer);
		loseSound.setVolume(60.f);
	}

	if (!winBuffer.loadFromFile("assets/win.wav"))
		std::cerr << "Failed to load win.wav\n";
	else {
		winSound.setBuffer(winBuffer);
		winSound.setVolume(60.f);
	}

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

    if (state == GameState::Menu) {
        titleTime += dt * titlePulseSpeed;
        float alpha = 200 + std::sin(titleTime) * 55;
        titleText.setFillColor(sf::Color(100, 200, 255, static_cast<sf::Uint8>(alpha)));

        instructionText.setPosition(220, 300 + std::sin(titleTime * 1.5f) * 5);
        return;
    }


	player.update(dt, &map);

	if (map.collectCrystalAt(player.getBounds()))
		collectSound.play();

	for (auto& e : enemies)
		e->update(dt, &map, player.getPosition());

	for (auto& enemy : enemies) {
		if (player.getBounds().intersects(enemy->getBounds())) {
			state = GameState::GameOver;
			statusText.setString("Game Over! You were caught! Press R to restart.");
			statusText.setFillColor(sf::Color::Red);
			loseSound.play();
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
		winSound.play();
	}
	scoreText.setString("Score: " + std::to_string(map.getScore()));

}

void Game::draw() {
	window.clear();
	window.draw(background);

	// compute offset to center map inside window
	float mapPixelW = static_cast<float>(map.getWidth()) * map.getTileSize();
	float mapPixelH = static_cast<float>(map.getHeight()) * map.getTileSize();
	sf::Vector2f mapOffset((window.getSize().x - mapPixelW) * 0.5f, (window.getSize().y - mapPixelH) * 0.5f);

	if (state == GameState::Menu) {
		window.draw(titleText);
		window.draw(instructionText);
	}
	else if (state == GameState::Playing) {
		map.draw(window, mapOffset);
		player.draw(window, mapOffset);
		for (auto& enemy : enemies)
			enemy->draw(window, mapOffset);
		window.draw(scoreText);
	}else if(state == GameState::GameOver || state == GameState::Win) {
		map.draw(window, mapOffset);
		player.draw(window, mapOffset);
		for (auto& enemy : enemies)
			enemy->draw(window, mapOffset);
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
	enemies.push_back(std::make_unique<Random>("assets/ghost1.png", sf::Vector2f(300, 300), 100.f));
	enemies.push_back(std::make_unique<Chaser>("assets/ghost2.png", sf::Vector2f(300, 300), 120.f));
	enemies.push_back(std::make_unique<Learner>("assets/ghost3.png", sf::Vector2f(400, 500), 80.f));
	enemies.push_back(std::make_unique<Learner>("assets/ghost4.png", sf::Vector2f(600, 600), 80.f));
	for (auto& enemy : enemies) {
		enemy->scaleToTileSize(map.getTileSize());
		enemy->setPosition(enemy->getPosition());
	}

	state = GameState::Playing;
}
