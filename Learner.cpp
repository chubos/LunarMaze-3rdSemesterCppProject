#include "Learner.h" // Do³¹czenie nag³ówka klasy Learner.
#include <random> // Dla generowania liczb losowych (fallback i unikanie zaklinowania).
#include <cmath> // Dla funkcji std::abs (wartoœæ bezwzglêdna).
#include <array> // Dla kontenera std::array.
#include <algorithm> // Dla funkcji std::shuffle.

// Konstruktor klasy Learner.
Learner::Learner(const std::string& texPath, sf::Vector2f startPos, float spd)
	: Enemy(texPath, startPos, spd) // Wywo³anie konstruktora klasy bazowej Enemy.
{
}

// ## Aktualizacja AI dla Learner'a
void Learner::update(float dt, Map* map, std::optional<sf::Vector2f> playerPos)
{
	// Blok Fallback: Jeœli mapa lub pozycja gracza nie s¹ dostêpne, wykonaj prosty ruch losowy (jak Random).
	if (!map || !playerPos.has_value()) {
		// Statyczny generator liczb losowych (inicjalizowany raz).
		static std::mt19937 rng{ std::random_device{}() };
		changeDirTimer += dt; // Zwiêkszenie licznika czasu.

		if (changeDirTimer > changeDirInterval) { // Czy nadszed³ czas na zmianê kierunku?
			changeDirTimer = 0.f;
			std::uniform_int_distribution<int> dist(0, 3); // Dystrybucja losowa dla 4 kierunków.
			int d = dist(rng);

			// Ustawienie nowego kierunku osiowego.
			if (d == 0) direction = { 1.f, 0.f }; //prawo
			else if (d == 1) direction = { -1.f, 0.f }; // lewo
			else if (d == 2) direction = { 0.f, 1.f }; // dó³
			else direction = { 0.f, -1.f }; //gora
		}

		sf::Vector2f newPos = position + direction * speed * dt; // Obliczenie nowej pozycji.
		if (canMoveTo(*map, newPos)) // Jeœli ruch jest mo¿liwy.
			position = newPos; // Wykonaj ruch.

		setPosition(position); // Zaktualizuj pozycjê duszka.
		return;
	}

	// --- G£ÓWNA LOGIKA AI (UCZENIE SIÊ) ---

	// 1. OBLICZANIE MAPY CIEP£A

	// Przekszta³cenie pozycji gracza na wspó³rzêdne kafelka (int x, int y).
	int px = static_cast<int>(playerPos->x / map->getTileSize());
	int py = static_cast<int>(playerPos->y / map->getTileSize());

	auto key = std::make_pair(px, py);
	visitHeatmap[key] += 4; //Zwiêksz "ciep³o" obecnego kafelka 4-krotnie (szybsza reakcja na nowe dane)

	// Starzenie danych: zmniejsz ciep³o wszystkich kafelków o 1.
	for (auto& kv : visitHeatmap)
		kv.second = std::max(0, kv.second - 1); // Ciep³o nie mo¿e spaœæ poni¿ej 0.


	// 2. WYBÓR NAJGORÊTSZEGO CELU

	std::pair<int, int> best = key; // Domyœlnie obecna pozycja gracza.
	int bestVal = visitHeatmap[key]; // Domyœlna wartoœæ ciep³a.

	// Szukanie kafelka z maksymaln¹ wartoœci¹ ciep³a (czyli miejsca, gdzie gracz najczêœciej by³).
	for (auto& kv : visitHeatmap) {
		if (kv.second > bestVal) {
			best = kv.first; // Najgorêtszy kafelek.
			bestVal = kv.second;
		}
	}

	// Obliczenie pozycji w pikselach dla najgorêtszego celu.
	sf::Vector2f target(best.first * map->getTileSize(),
		best.second * map->getTileSize());

	sf::Vector2f diff = target - position; // Wektor ró¿nicy (kierunek do celu).

	// 3. RUCH TYLKO PROSTOPAD£Y W KIERUNKU CELU

	sf::Vector2f primaryDir{ 0.f, 0.f }; // Kierunek priorytetowy (np. X).
	sf::Vector2f secondaryDir{ 0.f, 0.f }; // Kierunek drugorzêdny (np. Y).

	// Sprawdzenie, który kierunek (X czy Y) jest silniejszy.
	if (std::abs(diff.x) > std::abs(diff.y)) {
		// Priorytet X: ruch w prawo/lewo.
		primaryDir = { (diff.x > 0) ? 1.f : -1.f, 0.f };
		// Drugorzêdny Y: ruch w górê/dó³.
		secondaryDir = { 0.f, (diff.y > 0) ? 1.f : -1.f };
	}
	else {
		// Priorytet Y: ruch w górê/dó³.
		primaryDir = { 0.f, (diff.y > 0) ? 1.f : -1.f };
		// Drugorzêdny X: ruch w prawo/lewo.
		secondaryDir = { (diff.x > 0) ? 1.f : -1.f, 0.f };
	}

	sf::Vector2f try1 = position + primaryDir * speed * dt; // Próba ruchu priorytetowego.
	sf::Vector2f try2 = position + secondaryDir * speed * dt; // Próba ruchu drugorzêdnego.

	if (canMoveTo(*map, try1)) // Jeœli priorytetowy ruch jest mo¿liwy.
		position = try1; // Wykonaj go.
	else if (canMoveTo(*map, try2)) // W przeciwnym razie, jeœli ruch drugorzêdny jest mo¿liwy.
		position = try2; // Wykonaj go.
	else {
		// Jeœli utkn¹³: spróbuj losowych, osiowych kierunków (unikanie zaklinowania).
		static std::mt19937 rng{ std::random_device{}() };
		std::array<sf::Vector2f, 4> dirs = {
			sf::Vector2f{1.f,0.f}, sf::Vector2f{-1.f,0.f},
			sf::Vector2f{0.f,1.f}, sf::Vector2f{0.f,-1.f}
		};

		std::shuffle(dirs.begin(), dirs.end(), rng); // Mieszanie kierunków.
		for (auto& d : dirs) { // Próba wylosowanych kierunków.
			sf::Vector2f t = position + d * speed * dt;
			if (canMoveTo(*map, t)) { // Jeœli ruch jest dozwolony.
				position = t; // Wykonaj ruch.
				break;
			}
		}
	}

	setPosition(position); // Zaktualizuj pozycjê duszka.
}