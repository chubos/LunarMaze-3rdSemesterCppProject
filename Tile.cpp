#include "Tile.h"
#include <iostream>

namespace { // Statyczne zmienne przechowuj¹ce tekstury i stan ³adowania.
	static sf::Texture wallTexture;
	static sf::Texture floorTexture;
	static sf::Texture crystalTexture;
	static bool texturesLoaded = false;
	static bool wallTexOk = false;
	static bool floorTexOk = false;
	static bool crystalTexOk = false;

	void ensureTileTexturesLoaded() 
	{
		if (texturesLoaded) return; 
		
		wallTexOk = wallTexture.loadFromFile("assets/wall.png"); // Próba ³adowania tekstur z plików i ustawienie flag sukcesu/b³êdu.
		if (!wallTexOk) std::cerr << "Nie udalo sie zaladowac assets/wall.png\n";
		floorTexOk = floorTexture.loadFromFile("assets/floor.png");
		if (!floorTexOk) std::cerr << "Nie udalo sie zaladowac assets/floor.png\n";
		crystalTexOk = crystalTexture.loadFromFile("assets/crystal.png");
		if (!crystalTexOk) std::cerr << "Nie udalo sie zaladowac assets/crystal.png\n";

		texturesLoaded = true;
	}
}

Tile::Tile(TileType type, sf::Vector2f position, float size)
	: type(type), tileSize(size)
{
	shape.setSize({ size, size }); 
	shape.setPosition(position); 

	if (type == TileType::Wall) // Ustawienie domyœlnego koloru (na wypadek braku tekstury).
		shape.setFillColor(sf::Color(60, 60, 80));
	else if (type == TileType::Floor)
		shape.setFillColor(sf::Color(20, 20, 40));
	else if (type == TileType::Crystal)
		shape.setFillColor(sf::Color(0, 200, 255));

	ensureTileTexturesLoaded(); 

	if (type == TileType::Wall && wallTexOk) { 
		useTexture = true;
		sprite.setTexture(wallTexture, true);
	}
	else if (type == TileType::Floor && floorTexOk) {
		useTexture = true;
		sprite.setTexture(floorTexture, true); 
	}
	else if (type == TileType::Crystal && crystalTexOk) {
		useTexture = true;
		sprite.setTexture(crystalTexture, true); 
	}
	else {
		useTexture = false; 
	}

	
	if (useTexture) { 
		auto texSize = sprite.getTexture()->getSize();
		if (texSize.x == 0 || texSize.y == 0) {
			useTexture = false;
		}
		else {
			float sx = size / static_cast<float>(texSize.x);
			float sy = size / static_cast<float>(texSize.y);
			sprite.setScale(sx, sy);
			sprite.setOrigin(0.f, 0.f); // Ustawienie punktu pocz¹tkowego na górny lewy róg.
			sprite.setPosition(position); // Ustawienie pozycji sprite.
		}
	}
}


void Tile::draw(sf::RenderWindow& window, sf::Vector2f offset) { 
	sf::RenderStates states;
	states.transform.translate(offset);

	if (useTexture) {
		window.draw(sprite, states); // Rysowanie sprite.
	}
	else {
		window.draw(shape, states); // Rysowanie prostok¹ta.
	}
}


TileType Tile::getType() const 
{
	return type;
}


bool Tile::isCollected() const
{
	return collected;
}


void Tile::collect()
{
	collected = true; 

	if (type == TileType::Crystal) { 
		type = TileType::Floor; 

		if (floorTexOk) { 
			useTexture = true;
			sprite.setTexture(floorTexture, true); 

			
			auto texSize = sprite.getTexture()->getSize();
			if (texSize.x > 0 && texSize.y > 0) {
				sprite.setScale(tileSize / static_cast<float>(texSize.x),
					tileSize / static_cast<float>(texSize.y));
				sprite.setOrigin(0.f, 0.f);
				sprite.setPosition(shape.getPosition());
			}
		}
		else { 
			useTexture = false;
			shape.setFillColor(sf::Color(20, 20, 40)); 
		}
	}
}


sf::FloatRect Tile::getBounds() const { 
	if (useTexture) return sprite.getGlobalBounds(); 
	return shape.getGlobalBounds();
}