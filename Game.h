#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Map.h"

class Game {
	sf::RenderWindow window;
	Map map;
	Player player;
	std::vector<std::unique_ptr<Enemy>> enemies;

public:
	Game();
	void run();
};