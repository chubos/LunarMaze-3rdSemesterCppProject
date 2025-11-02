#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Map.h"

enum class GameState {
	Menu,
	Playing,
	Win,
	GameOver
};

class Game {
	sf::RenderWindow window;
	Map map;
	Player player;
	std::vector<std::unique_ptr<Enemy>> enemies;

	sf::Font font;
	sf::Text scoreText;
	sf::Text statusText;
	sf::Text titleText;
	sf::Text instructionText;

	GameState state = GameState::Menu;

public:
	Game();
	void run();

private:
	void handleEvents();
	void update(float dt);
	void draw();
	void resetGame();
};