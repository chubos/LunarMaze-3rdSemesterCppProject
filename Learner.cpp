#include "Learner.h"
#include <random>
#include <cmath>
#include <array>
#include <algorithm>

Learner::Learner(const std::string& texPath, sf::Vector2f startPos, float spd)
	: Enemy(texPath, startPos, spd)
{
}


void Learner::update(float dt, Map* map, std::optional<sf::Vector2f> playerPos)
{
	if (!map || !playerPos.has_value()) { // Jeœli mapa lub pozycja gracza nie s¹ dostêpne, wykonaj prosty ruch losowy (jak Random).
		static std::mt19937 rng{ std::random_device{}() };
		changeDirTimer += dt;

		if (changeDirTimer > changeDirInterval) {
			changeDirTimer = 0.f;
			std::uniform_int_distribution<int> dist(0, 3); // Dystrybucja losowa dla 4 kierunków.
			int d = dist(rng);

			// Ustawienie nowego kierunku
			if (d == 0) direction = { 1.f, 0.f }; // prawo
			else if (d == 1) direction = { -1.f, 0.f }; // lewo
			else if (d == 2) direction = { 0.f, 1.f }; // dó³
			else direction = { 0.f, -1.f }; //gora
		}

		sf::Vector2f newPos = position + direction * speed * dt;
		if (canMoveTo(*map, newPos))
			position = newPos;

		setPosition(position); // Zaktualizuj pozycjê duszka.
		return;
	}


	// OBLICZANIE MAPY CIEP£A

	int px = static_cast<int>(playerPos->x / map->getTileSize()); // Przekszta³cenie pozycji gracza na wspó³rzêdne kafelka (int x, int y).
	int py = static_cast<int>(playerPos->y / map->getTileSize());

	auto key = std::make_pair(px, py);
	visitHeatmap[key] += 2;

	
	for (auto& kv : visitHeatmap) // Starzenie danych: zmniejsz ciep³o wszystkich kafelków o 1.
		kv.second = std::max(0, kv.second - 1);


	// WYBÓR NAJGORÊTSZEGO CELU

	std::pair<int, int> best = key; // Domyœlnie obecna pozycja gracza.
	int bestVal = visitHeatmap[key]; // Domyœlna wartoœæ ciep³a.

	
	for (auto& kv : visitHeatmap) { // Szukanie kafelka z maksymaln¹ wartoœci¹ ciep³a.
		if (kv.second > bestVal) {
			best = kv.first; // Najgorêtszy kafelek.
			bestVal = kv.second;
		}
	}

	
	sf::Vector2f target(best.first * map->getTileSize(), // Obliczenie pozycji w pikselach dla najgorêtszego celu.
		best.second * map->getTileSize());

	sf::Vector2f diff = target - position; // Wektor ró¿nicy (kierunek do celu).

	// RUCH TYLKO PROSTOPAD£Y W KIERUNKU CELU

	sf::Vector2f primaryDir{ 0.f, 0.f }; // Kierunek priorytetowy (np. X).
	sf::Vector2f secondaryDir{ 0.f, 0.f }; // Kierunek drugorzêdny (np. Y).

	if (std::abs(diff.x) > std::abs(diff.y)) { // Sprawdzenie, który kierunek (X czy Y) jest silniejszy.
		primaryDir = { (diff.x > 0) ? 1.f : -1.f, 0.f }; // Priorytet X: ruch w prawo/lewo.
		secondaryDir = { 0.f, (diff.y > 0) ? 1.f : -1.f }; // Drugorzêdny Y: ruch w górê/dó³.
	} else {
		primaryDir = { 0.f, (diff.y > 0) ? 1.f : -1.f }; // Priorytet Y: ruch w górê/dó³.
		secondaryDir = { (diff.x > 0) ? 1.f : -1.f, 0.f }; // Drugorzêdny X: ruch w prawo/lewo.
	}

	sf::Vector2f try1 = position + primaryDir * speed * dt; // Próba ruchu priorytetowego.
	sf::Vector2f try2 = position + secondaryDir * speed * dt; // Próba ruchu drugorzêdnego.

	if (canMoveTo(*map, try1)) // Jeœli priorytetowy ruch jest mo¿liwy.
		position = try1;
	else if (canMoveTo(*map, try2)) // W przeciwnym razie, jeœli ruch drugorzêdny jest mo¿liwy.
		position = try2;
	else {
		// Jeœli utkn¹³: spróbuj losowych, osiowych kierunków.
		static std::mt19937 rng{ std::random_device{}() };
		std::array<sf::Vector2f, 4> dirs = {
			sf::Vector2f{1.f,0.f}, sf::Vector2f{-1.f,0.f},
			sf::Vector2f{0.f,1.f}, sf::Vector2f{0.f,-1.f}
		};

		std::shuffle(dirs.begin(), dirs.end(), rng); // Mieszanie kierunków.
		for (auto& d : dirs) {
			sf::Vector2f t = position + d * speed * dt;
			if (canMoveTo(*map, t)) {
				position = t; 
				break;
			}
		}
	}

	setPosition(position); // Zaktualizuj pozycjê duszka.
}