#include "Random.h" // Dołączenie nagłówka klasy Random.
#include <random> // Dołączenie biblioteki dla generatora liczb losowych.
#include <vector> // Dołączenie dla std::vector.
#include <algorithm> // Dołączenie dla std::shuffle.

// Statyczna inicjalizacja generatora liczb losowych (rng)
std::mt19937 Random::rng{ std::random_device{}() };

// ## Konstruktor klasy Random
Random::Random(const std::string& texPath, sf::Vector2f startPos, float spd)
	: Enemy(texPath, startPos, spd) // Wywołanie konstruktora klasy bazowej Enemy.
{
}

// ## Metoda aktualizacji AI dla Losowego wroga
void Random::update(float dt, Map* map, std::optional<sf::Vector2f>)
{
	if (!map) return; // Jeśli brakuje mapy, przerwij.

	changeDirTimer += dt; // Aktualizacja timera zmiany kierunku.

	// --- Losowa zmiana kierunku co jakiś czas ---
	if (changeDirTimer > changeDirInterval) {
		changeDirTimer = 0.f; // Zresetowanie timera.

		// Wybór kierunku: góra/dół/lewo/prawo (0-3).
		std::uniform_int_distribution<int> d(0, 3);
		int dir = d(rng); // Wylosowanie nowego kierunku.

		switch (dir) {
		case 0: direction = { 1.f, 0.f }; break; //prawo
		case 1: direction = { -1.f, 0.f }; break; // lewo
		case 2: direction = { 0.f, 1.f }; break; //dol
		case 3: direction = { 0.f, -1.f }; break; // góra
		}
	}

	sf::Vector2f newPos = position + direction * speed * dt; // Obliczenie docelowej pozycji.

	// --- Jeżeli może iść prosto, to idzie ---
	if (canMoveTo(*map, newPos)) { // Sprawdzenie, czy ruch w obecnym kierunku jest możliwy.
		position = newPos; // Wykonanie ruchu.
		setPosition(position); // Aktualizacja pozycji duszka.
		return; // Zakończ aktualizację.
	}

	// ======================================================
	//   KOLIZJA → WYBÓR KIERUNKU PROSTOPADŁEGO (LUB DRUGIEGO)
	// ======================================================

	std::vector<sf::Vector2f> perpendicularDirs; // Kontener na prostopadłe kierunki.

	// Jeśli ruch jest poziomy (x != 0) → próbujemy kierunki pionowe.
	if (direction.x != 0) {
		perpendicularDirs.push_back({ 0.f, 1.f }); // dol
		perpendicularDirs.push_back({ 0.f, -1.f }); // góra
	}
	// Jeśli ruch jest pionowy (y != 0) → próbujemy kierunki poziome.
	if (direction.y != 0) {
		perpendicularDirs.push_back({ 1.f, 0.f });//prawo
		perpendicularDirs.push_back({ -1.f, 0.f }); // lewo
	}

	// Tasowanie kierunków, aby wybór był losowy.
	std::shuffle(perpendicularDirs.begin(), perpendicularDirs.end(), rng);

	// Próba przejścia prostopadłego.
	for (auto& ndir : perpendicularDirs) {
		sf::Vector2f testPos = position + ndir * speed * dt; // Wyliczenie pozycji testowej.
		if (canMoveTo(*map, testPos)) { // Jeśli ruch jest możliwy.
			direction = ndir; // Zmień kierunek na nowy, prostopadły.
			position = testPos; // Wykonaj ruch.
			setPosition(position); // Aktualizacja duszka.
			return; // Zakończ.
		}
	}

	// Jeśli prostopadłe nie działają → ostatnia opcja: odwrót (ruch o 180 stopni).
	sf::Vector2f backDir = -direction; // Kierunek przeciwny do obecnego.
	sf::Vector2f backPos = position + backDir * speed * dt;
	if (canMoveTo(*map, backPos)) {
		direction = backDir; // Ustaw kierunek na przeciwny.
		position = backPos; // Wykonaj ruch.
		setPosition(position); // Aktualizacja duszka.
		return; // Zakończ.
	}

	// W skrajnych sytuacjach stoi, ale to jest mało prawdopodobne w otwartym labiryncie.
	setPosition(position);
}