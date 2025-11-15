#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "Leaderboard.h"
#include "RoundedRectangle.h" // Do³¹czamy now¹ klasê

enum class GameState {
	Menu,
	Playing,
	LeaderboardView,
	EnterName,
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

	// Elementy UI w Menu (Zmienione na RoundedRectangle)
	RoundedRectangle startButton;
	sf::Text startButtonText;
	RoundedRectangle leaderboardButton;
	sf::Text leaderboardButtonText;

	// Elementy UI w widoku Tabeli Wyników (Zmienione na RoundedRectangle)
	Leaderboard leaderboard;
	RoundedRectangle backButton;
	sf::Text backButtonText;
	RoundedRectangle resetButton;
	sf::Text resetButtonText;

	// Elementy UI wprowadzania imienia (EnterName) (Zmienione na RoundedRectangle)
	std::string inputName;
	int pendingScore = 0;
	sf::Text inputPromptText;
	sf::Text inputText;
	RoundedRectangle submitButton;
	sf::Text submitButtonText;
	RoundedRectangle cancelButton;
	sf::Text cancelButtonText;

	GameState prevEndState = GameState::Menu;

	sf::Texture backgroundTexture;
	sf::Sprite background;
	sf::Image backgroundImage;
	bool backgroundImageLoaded = false;

	sf::Music music;
	sf::SoundBuffer collectBuffer, loseBuffer, winBuffer;
	sf::Sound collectSound, loseSound, winSound;

	GameState state = GameState::Menu;

	float titleTime = 0.f;
	float titlePulseSpeed = 2.f;

	float inputDelay = 0.f;
	float inputDelayDuration = 1.25f;

public:
	Game();
	void run();

private:
	void handleEvents();
	void update(float dt);
	void draw();
	void resetGame();

	void openLeaderboard();
	void submitHighScore();
	void cancelHighScore();
};