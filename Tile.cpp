#include "Tile.h"
#include <iostream>

namespace { // Statyczne zmienne przechowuj¹ce tekstury i stan ³adowania.
	static sf::Texture wallTexture; // Tekstura œciany.
	static sf::Texture floorTexture; // Tekstura pod³ogi.
	static sf::Texture crystalTexture; // Tekstura kryszta³u.
	static bool texturesLoaded = false; // Flaga, czy podjêto próbê za³adowania tekstur.
	static bool wallTexOk = false; // Flaga, czy ³adowanie tekstury œciany siê powiod³o.
	static bool floorTexOk = false; // Flaga, czy ³adowanie tekstury pod³ogi siê powiod³o.
	static bool crystalTexOk = false; // Flaga, czy ³adowanie tekstury kryszta³u siê powiod³o.

	void ensureTileTexturesLoaded() 	// Funkcja ³adowania tekstur.
	{
		if (texturesLoaded) return; // Jeœli ju¿ za³adowano, wyjdŸ.
		
		wallTexOk = wallTexture.loadFromFile("assets/wall.png"); // Próba ³adowania tekstur z plików i ustawienie flag sukcesu/b³êdu.
		if (!wallTexOk) std::cerr << "Nie udalo sie zaladowac assets/wall.png\n";
		floorTexOk = floorTexture.loadFromFile("assets/floor.png");
		if (!floorTexOk) std::cerr << "Nie udalo sie zaladowac assets/floor.png\n";
		crystalTexOk = crystalTexture.loadFromFile("assets/crystal.png");
		if (!crystalTexOk) std::cerr << "Nie udalo sie zaladowac assets/crystal.png\n";

		texturesLoaded = true; // Ustaw flagê, ¿e ³adowanie zosta³o przeprowadzone.
	}
}

Tile::Tile(TileType type, sf::Vector2f position, float size)
	: type(type), tileSize(size) // Inicjalizacja typu kafelka i rozmiaru.
{
	shape.setSize({ size, size }); // Ustawienie rozmiaru kafelka.
	shape.setPosition(position); // Ustawienie pozycji kafelka.

	if (type == TileType::Wall) // Ustawienie domyœlnego koloru (na wypadek braku tekstury).
		shape.setFillColor(sf::Color(60, 60, 80));
	else if (type == TileType::Floor)
		shape.setFillColor(sf::Color(20, 20, 40));
	else if (type == TileType::Crystal)
		shape.setFillColor(sf::Color(0, 200, 255));

	ensureTileTexturesLoaded(); // Upewnij siê, ¿e tekstury s¹ za³adowane.

	if (type == TileType::Wall && wallTexOk) { // Przypisanie tekstury do sprite'a, jeœli ³adowanie siê powiod³o.
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

	
	if (useTexture) { // Jeœli u¿ywamy tekstury, przeskaluj j¹, aby pasowa³a do rozmiaru kafelka.
		auto texSize = sprite.getTexture()->getSize();
		if (texSize.x == 0 || texSize.y == 0) { // Zabezpieczenie przed zerowym rozmiarem.
			useTexture = false;
		}
		else {
			float sx = size / static_cast<float>(texSize.x); // Obliczenie skali X.
			float sy = size / static_cast<float>(texSize.y); // Obliczenie skali Y.
			sprite.setScale(sx, sy); // Zastosowanie skalowania.
			sprite.setOrigin(0.f, 0.f); // Ustawienie punktu pocz¹tkowego na górny lewy róg.
			sprite.setPosition(position); // Ustawienie pozycji sprite.
		}
	}
}


void Tile::draw(sf::RenderWindow& window, sf::Vector2f offset) { // Rysowanie kafelka
	sf::RenderStates states;
	states.transform.translate(offset); // Dodanie przesuniêcia.

	if (useTexture) {
		window.draw(sprite, states); // Rysowanie sprite.
	}
	else {
		window.draw(shape, states); // Rysowanie kszta³tu (prostok¹ta).
	}
}


TileType Tile::getType() const // Pobieranie typu kafelka
{
	return type;
}


bool Tile::isCollected() const // Sprawdzenie, czy kafelek zosta³ zebrany
{
	return collected;
}


void Tile::collect() // Logika zbierania kryszta³u
{
	collected = true; // Ustaw flagê, ¿e zosta³ zebrany.

	if (type == TileType::Crystal) { 
		type = TileType::Floor; // Zmieñ typ na Pod³ogê.

		if (floorTexOk) { // Jeœli tekstura pod³ogi jest dostêpna
			useTexture = true;
			sprite.setTexture(floorTexture, true); // Zmieñ teksturê na pod³ogê.

			
			auto texSize = sprite.getTexture()->getSize(); // Ponowne skalowanie tekstury.
			if (texSize.x > 0 && texSize.y > 0) {
				sprite.setScale(tileSize / static_cast<float>(texSize.x),
					tileSize / static_cast<float>(texSize.y));
				sprite.setOrigin(0.f, 0.f);
				sprite.setPosition(shape.getPosition());
			}
		}
		else { // Jeœli tekstura pod³ogi nie jest dostêpna
			useTexture = false;
			shape.setFillColor(sf::Color(20, 20, 40)); // Ustaw kolor Pod³ogi.
		}
	}
}


sf::FloatRect Tile::getBounds() const { // Pobieranie granic kafelka.
	if (useTexture) return sprite.getGlobalBounds(); // Zwróæ granice na podstawie u¿ywanego obiektu.
	return shape.getGlobalBounds();
}