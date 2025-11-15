#include "Chaser.h" // Do³¹czenie pliku nag³ówkowego klasy Chaser.
#include <array> // Do³¹czenie nag³ówka dla std::array.

// ## Konstruktor klasy Chaser
// Wywo³uje konstruktor klasy bazowej Enemy.
Chaser::Chaser(const std::string& texPath, sf::Vector2f startPos, float spd)
	: Enemy(texPath, startPos, spd)
{
	// Logika ruchu "Chaser" jest w pe³ni zaimplementowana w metodzie update.
}

// ## Metoda aktualizacji AI dla Chaser'a (Goni¹cego)
// Implementuje AI, które polega na wyborze kierunku minimalizuj¹cego dystans do gracza.
void Chaser::update(float dt, Map* map, std::optional<sf::Vector2f> playerPos)
{
	// Zabezpieczenie: jeœli mapa lub pozycja gracza nie s¹ dostêpne, przerywamy.
	if (!map || !playerPos.has_value()) return;

	sf::Vector2f player = playerPos.value(); // Aktualna pozycja gracza.
	sf::Vector2f bestDir = direction; // Zaczynamy od obecnego kierunku jako najlepszego.

	// ===========================================
	// 1. Rozwa¿amy tylko 4 kierunki osiowe (Góra, Dó³, Lewo, Prawo).
	// ===========================================
	std::array<sf::Vector2f, 4> dirs = {
		sf::Vector2f(1.f,  0.f),  // Prawo
		sf::Vector2f(-1.f,  0.f),  // Lewo
		sf::Vector2f(0.f,  1.f),  // Dó³
		sf::Vector2f(0.f, -1.f)   // Góra
	};

	float bestDist = 999999.f; // Pocz¹tkowo bardzo du¿y dystans.

	// =======================================================
	// 2. Wybieramy kierunek, który zmniejsza dystans do gracza
	//    i jest mo¿liwy do wykonania (nie koliduje ze œcian¹).
	// =======================================================
	for (auto& d : dirs)
	{
		// Obliczenie potencjalnej nowej pozycji (np) po wykonaniu ruchu w kierunku 'd'.
		sf::Vector2f np = position + d * speed * dt;

		// Pomijamy kierunki, które prowadz¹ do kolizji ze œcian¹.
		if (!canMoveTo(*map, np))
			continue;

		// Obliczenie kwadratu dystansu euklidesowego do gracza z nowej pozycji.
		// U¿ycie kwadratu dystansu (dist = dx*dx + dy*dy) jest szybsze ni¿ pierwiastek.
		float dx = player.x - np.x;
		float dy = player.y - np.y;
		float dist = dx * dx + dy * dy;

		// Jeœli znaleziono lepszy (mniejszy) dystans:
		if (dist < bestDist) {
			bestDist = dist; // Zapisujemy nowy minimalny dystans.
			bestDir = d; // Zapisujemy ten kierunek jako najlepszy.
		}
	}

	direction = bestDir; // Ustawienie wybranego kierunku ruchu.

	// ===========================================
	// 3. W³aœciwe wykonanie ruchu.
	// ===========================================
	sf::Vector2f newPos = position + direction * speed * dt;

	// Sprawdzenie, czy ruch w wybranym kierunku jest mo¿liwy.
	if (canMoveTo(*map, newPos)) {
		position = newPos; // Wykonanie ruchu.
	}
	else {
		// Jeœli najlepszy ruch jest zablokowany (np. przez œcianê) –
		// Awaryjne sprawdzenie pozosta³ych kierunków, aby unikn¹æ totalnego zatrzymania.
		for (auto& d : dirs) {
			sf::Vector2f np = position + d * speed * dt;
			// Jeœli inny ruch jest mo¿liwy, wybieramy go i przerywamy.
			if (canMoveTo(*map, np)) {
				direction = d;
				position = np;
				break;
			}
		}
	}

	// Aktualizacja pozycji duszka (sprite) po zmianie logicznej pozycji.
	setPosition(position);
}