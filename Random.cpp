#include "Random.h"
#include <random>
#include <vector>
#include <algorithm>

std::mt19937 Random::rng{ std::random_device{}() };

Random::Random(const std::string& texPath, sf::Vector2f startPos, float spd)
	: Enemy(texPath, startPos, spd)
{
}


void Random::update(float dt, Map* map, std::optional<sf::Vector2f>)
{
	if (!map) return;

	changeDirTimer += dt;

	if (changeDirTimer > changeDirInterval) {
		changeDirTimer = 0.f;

		std::uniform_int_distribution<int> d(0, 3); // Wybór kierunku.
		int dir = d(rng); // Wylosowanie nowego kierunku.

		switch (dir) {
		case 0: direction = { 1.f, 0.f }; break; // Prawo.
		case 1: direction = { -1.f, 0.f }; break; // Lewo.
		case 2: direction = { 0.f, 1.f }; break; // Dół.
		case 3: direction = { 0.f, -1.f }; break; // Góra.
		}
	}

	sf::Vector2f newPos = position + direction * speed * dt;

	if (canMoveTo(*map, newPos)) { 
		position = newPos;
		setPosition(position);
		return;
	}

	// Wybór kierunku prostopadłego w przypadku kolizji.

	std::vector<sf::Vector2f> perpendicularDirs; // Wektor na prostopadłe kierunki.

	
	if (direction.x != 0) { // Jeśli ruch jest poziomy, próbujemy kierunki pionowe.
		perpendicularDirs.push_back({ 0.f, 1.f }); // Dół.
		perpendicularDirs.push_back({ 0.f, -1.f }); // Góra.
	}

	if (direction.y != 0) { // Jeśli ruch jest pionowy, próbujemy kierunki poziome.
		perpendicularDirs.push_back({ 1.f, 0.f }); // Prawo.
		perpendicularDirs.push_back({ -1.f, 0.f }); // Lewo.
	}

	std::shuffle(perpendicularDirs.begin(), perpendicularDirs.end(), rng);


	for (auto& ndir : perpendicularDirs) {
		sf::Vector2f testPos = position + ndir * speed * dt;
		if (canMoveTo(*map, testPos)) {
			direction = ndir; 
			position = testPos; 
			setPosition(position); 
			return;
		}
	}

	// Jeśli prostopadłe nie działają, to odwrót (ruch o 180 stopni).
	sf::Vector2f backDir = -direction;
	sf::Vector2f backPos = position + backDir * speed * dt;
	if (canMoveTo(*map, backPos)) {
		direction = backDir; 
		position = backPos; 
		setPosition(position); 
		return;
	}

	setPosition(position); // Ustawienie pozycji bez ruchu, jeśli wszystkie opcje zawiodły.
}