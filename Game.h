#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

	sf::Texture backgroundTexture;
	sf::Sprite background;

	sf::Music music;
	sf::SoundBuffer collectBuffer, loseBuffer, winBuffer;
	sf::Sound collectSound, loseSound, winSound;

	GameState state = GameState::Menu;

	float titleTime = 0.f;
	float titlePulseSpeed = 2.f;

public:
	Game();
	void run();

private:
	void handleEvents();
	void update(float dt);
	void draw();
	void resetGame();
};