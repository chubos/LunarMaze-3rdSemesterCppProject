#pragma once
#include <SFML/Graphics.hpp>

enum class TileType {
	Wall, // Œciana (nieprzechodnia).
	Floor, // Pod³oga (przechodnia).
	Crystal // Kryszta³.
};

class Tile
{
	TileType type; // Typ kafelka.
	sf::RectangleShape shape; // Kszta³t prostok¹tny (u¿ywany, jeœli nie ma tekstury).
	sf::Sprite sprite; // Sprite (u¿ywany, jeœli jest tekstura).
	bool collected = false; // Czy kryszta³ zosta³ ju¿ zebrany.
	bool useTexture = false; // Flaga okreœlaj¹ca, czy u¿ywaæ tekstury (sprite) czy kszta³tu (shape).
	float tileSize = 0.f; // Rozmiar kafelka w pikselach.

public:
	Tile(TileType type, sf::Vector2f position, float size); // Konstruktor: ustawia typ, pozycjê i rozmiar kafelka.
	void draw(sf::RenderWindow& window, sf::Vector2f offset = { 0.f, 0.f }); 	// Rysuje kafelek na oknie, opcjonalnie z przesuniêciem (offset).

	TileType getType() const; // Zwraca typ kafelka.
	bool isCollected() const; // Zwraca status zebrania.
	void collect(); // Oznacza kafelek jako zebrany.
	sf::FloatRect getBounds() const; // Zwraca granice kafelka (do detekcji kolizji).
};