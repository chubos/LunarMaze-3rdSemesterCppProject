#include "Random.h"
#include <random>
#include <vector>
#include <algorithm>

std::mt19937 Random::rng{ std::random_device{}() }; // Statyczna inicjalizacja generatora liczb losowych (rng)

Random::Random(const std::string& texPath, sf::Vector2f startPos, float spd)
	: Enemy(texPath, startPos, spd) // Wywołanie konstruktora klasy bazowej Enemy.
{
}


void Random::update(float dt, Map* map, std::optional<sf::Vector2f>) // Metoda  AI dla Losowego wroga.
{
	if (!map) return; // Jeśli brakuje mapy, przerwij.

	changeDirTimer += dt; // Aktualizacja timera zmiany kierunku.

	if (changeDirTimer > changeDirInterval) { // Losowa zmiana kierunku co ustalony czas.
		changeDirTimer = 0.f; // Zresetowanie timera.

		std::uniform_int_distribution<int> d(0, 3); // Wybór kierunku: góra/dół/lewo/prawo (0-3).
		int dir = d(rng); // Wylosowanie nowego kierunku.

		switch (dir) {
		case 0: direction = { 1.f, 0.f }; break; // Prawo.
		case 1: direction = { -1.f, 0.f }; break; // Lewo.
		case 2: direction = { 0.f, 1.f }; break; // Dół.
		case 3: direction = { 0.f, -1.f }; break; // Góra.
		}
	}

	sf::Vector2f newPos = position + direction * speed * dt; // Obliczenie docelowej pozycji.

	if (canMoveTo(*map, newPos)) { // Sprawdzenie, czy ruch w obecnym kierunku jest możliwy.
		position = newPos; // Wykonanie ruchu.
		setPosition(position); // Aktualizacja pozycji duszka.
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

	std::shuffle(perpendicularDirs.begin(), perpendicularDirs.end(), rng); 	// Tasowanie kierunków, aby wybór był losowy.


	for (auto& ndir : perpendicularDirs) { 	// Próba przejścia prostopadłego.
		sf::Vector2f testPos = position + ndir * speed * dt; // Wyliczenie pozycji testowej.
		if (canMoveTo(*map, testPos)) { // Jeśli ruch jest możliwy.
			direction = ndir; // Zmień kierunek na nowy, prostopadły.
			position = testPos; // Wykonaj ruch.
			setPosition(position); // Aktualizacja duszka.
			return;
		}
	}

	// Jeśli prostopadłe nie działają, to odwrót (ruch o 180 stopni).
	sf::Vector2f backDir = -direction; // Kierunek przeciwny do obecnego.
	sf::Vector2f backPos = position + backDir * speed * dt;
	if (canMoveTo(*map, backPos)) {
		direction = backDir; // Ustaw kierunek na przeciwny.
		position = backPos; // Wykonaj ruch.
		setPosition(position); // Aktualizacja duszka.
		return;
	}

	setPosition(position); // Ustawienie pozycji bez ruchu, jeśli wszystkie opcje zawiodły.
}