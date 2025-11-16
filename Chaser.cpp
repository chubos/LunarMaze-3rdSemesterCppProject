#include "Chaser.h"
#include <array>

Chaser::Chaser(const std::string& texPath, sf::Vector2f startPos, float spd)
	: Enemy(texPath, startPos, spd)
{
}

void Chaser::update(float dt, Map* map, std::optional<sf::Vector2f> playerPos) // Implementuje AI, które polega na wyborze kierunku minimalizuj¹cego dystans do gracza.
{
	if (!map || !playerPos.has_value()) return; // Zabezpieczenie, jeœli mapa lub pozycja gracza nie s¹ dostêpne.

	sf::Vector2f player = playerPos.value(); // Aktualna pozycja gracza.
	sf::Vector2f bestDir = direction; // Zaczynamy od obecnego kierunku jako najlepszego.

	std::array<sf::Vector2f, 4> dirs = { // 4 kierunki osiowe.
		sf::Vector2f(1.f,  0.f),  // Prawo
		sf::Vector2f(-1.f,  0.f),  // Lewo
		sf::Vector2f(0.f,  1.f),  // Dó³
		sf::Vector2f(0.f, -1.f)   // Góra
	};

	float bestDist = 999999.f; // Pocz¹tkowo bardzo du¿y dystans.

	for (auto& d : dirs) // Wybieramy kierunek, który zmniejsza dystans do gracza i nie koliduje ze œcian¹.
	{
		sf::Vector2f np = position + d * speed * dt; // Obliczenie potencjalnej nowej pozycji (np) po wykonaniu ruchu w kierunku 'd'.

		if (!canMoveTo(*map, np)) // Pomijamy kierunki, które prowadz¹ do kolizji ze œcian¹.
			continue;

		float dx = player.x - np.x; // Obliczenie dystansu do gracza z nowej pozycji.
		float dy = player.y - np.y;
		float dist = dx * dx + dy * dy;

		
		if (dist < bestDist) { // Jeœli znaleziono lepszy (mniejszy) dystans:
			bestDist = dist; // Zapisujemy nowy minimalny dystans.
			bestDir = d; // Zapisujemy ten kierunek jako najlepszy.
		}
	}

	direction = bestDir; // Ustawienie wybranego kierunku ruchu.

	sf::Vector2f newPos = position + direction * speed * dt; // Wykonanie ruchu.

	if (canMoveTo(*map, newPos)) { // Sprawdzenie, czy ruch w wybranym kierunku jest mo¿liwy.
		position = newPos; // Wykonanie ruchu.
	}
	else {
		for (auto& d : dirs) { // Jeœli najlepszy ruch jest zablokowany – awaryjne sprawdzenie pozosta³ych kierunków, aby unikn¹æ zatrzymania.
			sf::Vector2f np = position + d * speed * dt;
			if (canMoveTo(*map, np)) { // Jeœli inny ruch jest mo¿liwy, wybieramy go i przerywamy.
				direction = d;
				position = np;
				break;
			}
		}
	}

	setPosition(position); // Aktualizacja pozycji duszka.
}