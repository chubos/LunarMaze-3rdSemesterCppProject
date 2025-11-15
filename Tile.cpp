#include "Tile.h" // Do³¹czenie nag³ówka klasy Tile.
#include <iostream> // Do operacji wejœcia/wyjœcia (std::cerr).

namespace {
	// Statyczne zmienne przechowuj¹ce tekstury i stan ³adowania.
	static sf::Texture wallTexture; // Tekstura œciany.
	static sf::Texture floorTexture; // Tekstura pod³ogi.
	static sf::Texture crystalTexture; // Tekstura kryszta³u.
	static bool texturesLoaded = false; // Flaga, czy podjêto próbê za³adowania tekstur.
	static bool wallTexOk = false; // Flaga, czy ³adowanie tekstury œciany siê powiod³o.
	static bool floorTexOk = false; // Flaga, czy ³adowanie tekstury pod³ogi siê powiod³o.
	static bool crystalTexOk = false; // Flaga, czy ³adowanie tekstury kryszta³u siê powiod³o.

	// Funkcja ³adowania tekstur (wywo³ywana tylko raz).
	void ensureTileTexturesLoaded()
	{
		if (texturesLoaded) return; // Jeœli ju¿ za³adowano, wyjdŸ.

		// Próba ³adowania tekstur z plików i ustawienie flag sukcesu/b³êdu.
		wallTexOk = wallTexture.loadFromFile("assets/wall.png");
		if (!wallTexOk) std::cerr << "Failed to load assets/wall.png\n"; // Raport b³êdu.
		floorTexOk = floorTexture.loadFromFile("assets/floor.png");
		if (!floorTexOk) std::cerr << "Failed to load assets/floor.png\n";
		crystalTexOk = crystalTexture.loadFromFile("assets/crystal.png");
		if (!crystalTexOk) std::cerr << "Failed to load assets/crystal.png\n";

		texturesLoaded = true; // Ustaw flagê, ¿e ³adowanie zosta³o przeprowadzone.
	}
}

// ## Konstruktor klasy Tile
Tile::Tile(TileType type, sf::Vector2f position, float size)
	: type(type), tileSize(size) // Inicjalizacja typu kafelka i rozmiaru.
{
	shape.setSize({ size, size }); // Ustawienie rozmiaru kafelka (dla sf::RectangleShape).
	shape.setPosition(position); // Ustawienie pozycji kafelka.

	// Ustawienie domyœlnego koloru (na wypadek braku tekstury).
	if (type == TileType::Wall)
		shape.setFillColor(sf::Color(60, 60, 80));
	else if (type == TileType::Floor)
		shape.setFillColor(sf::Color(20, 20, 40));
	else if (type == TileType::Crystal)
		shape.setFillColor(sf::Color(0, 200, 255));

	ensureTileTexturesLoaded(); // Upewnij siê, ¿e tekstury s¹ za³adowane.

	// Przypisanie tekstury do sprite'a, jeœli ³adowanie siê powiod³o.
	if (type == TileType::Wall && wallTexOk) {
		useTexture = true;
		sprite.setTexture(wallTexture, true); // Przypisz teksturê œciany.
	}
	else if (type == TileType::Floor && floorTexOk) {
		useTexture = true;
		sprite.setTexture(floorTexture, true); // Przypisz teksturê pod³ogi.
	}
	else if (type == TileType::Crystal && crystalTexOk) {
		useTexture = true;
		sprite.setTexture(crystalTexture, true); // Przypisz teksturê kryszta³u.
	}
	else {
		useTexture = false; // U¿yj prostok¹ta, jeœli tekstura jest niedostêpna.
	}

	// Jeœli u¿ywamy tekstury, przeskaluj j¹, aby pasowa³a do rozmiaru kafelka.
	if (useTexture) {
		auto texSize = sprite.getTexture()->getSize();
		if (texSize.x == 0 || texSize.y == 0) { // Zabezpieczenie przed zerowym rozmiarem.
			useTexture = false;
		}
		else {
			float sx = size / static_cast<float>(texSize.x); // Obliczenie skali X.
			float sy = size / static_cast<float>(texSize.y); // Obliczenie skali Y.
			sprite.setScale(sx, sy); // Zastosowanie skalowania.
			sprite.setOrigin(0.f, 0.f); // Ustawienie punktu pocz¹tkowego na górny lewy róg.
			sprite.setPosition(position); // Ustawienie pozycji sprite'a.
		}
	}
}

// ## Rysowanie kafelka
void Tile::draw(sf::RenderWindow& window, sf::Vector2f offset) {
	sf::RenderStates states;
	states.transform.translate(offset); // Dodanie przesuniêcia (kamery).

	if (useTexture) {
		window.draw(sprite, states); // Rysowanie sprite'a.
	}
	else {
		window.draw(shape, states); // Rysowanie kszta³tu (prostok¹ta).
	}
}

// ## Pobieranie typu kafelka
TileType Tile::getType() const
{
	return type;
}

// ## Sprawdzenie, czy kafelek zosta³ zebrany
bool Tile::isCollected() const
{
	return collected;
}

// ## Logika zbierania kafelka (np. kryszta³u)
void Tile::collect()
{
	collected = true; // Ustaw flagê, ¿e zosta³ zebrany.

	if (type == TileType::Crystal) { // Jeœli to by³ kryszta³:
		type = TileType::Floor; // Zmieñ typ na Pod³ogê.

		if (floorTexOk) { // Jeœli tekstura pod³ogi jest dostêpna:
			useTexture = true;
			sprite.setTexture(floorTexture, true); // Zmieñ teksturê na pod³ogê.

			// Ponowne skalowanie tekstury.
			auto texSize = sprite.getTexture()->getSize();
			if (texSize.x > 0 && texSize.y > 0) {
				sprite.setScale(tileSize / static_cast<float>(texSize.x),
					tileSize / static_cast<float>(texSize.y));
				sprite.setOrigin(0.f, 0.f);
				sprite.setPosition(shape.getPosition());
			}
		}
		else { // Jeœli tekstura pod³ogi nie jest dostêpna:
			useTexture = false;
			shape.setFillColor(sf::Color(20, 20, 40)); // Ustaw kolor Pod³ogi.
		}
	}
}

// ## Pobieranie granic kafelka
sf::FloatRect Tile::getBounds() const {
	// Zwróæ granice na podstawie u¿ywanego obiektu (sprite lub shape).
	if (useTexture) return sprite.getGlobalBounds();
	return shape.getGlobalBounds();
}