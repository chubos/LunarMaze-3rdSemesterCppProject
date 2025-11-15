#pragma once // Zapewnia, ¿e ten plik nag³ówkowy zostanie do³¹czony tylko raz.
// Do³¹czenie standardowej biblioteki SFML dla grafiki.
#include <SFML/Graphics.hpp>

// Wyliczenie (enum class) definiuj¹ce mo¿liwe typy kafelków na mapie.
enum class TileType {
	Wall, // Œciana (nieprzechodnia).
	Floor, // Pod³oga (przechodnia).
	Crystal // Kryszta³ / Przedmiot do zebrania.
};


// Klasa Tile (Kafelek) reprezentuje pojedynczy element na siatce mapy.
class Tile
{
	TileType type; // Typ kafelka.
	sf::RectangleShape shape; // Kszta³t prostok¹tny (u¿ywany, jeœli nie ma tekstury).
	sf::Sprite sprite; // Duszek/Sprite (u¿ywany, jeœli jest tekstura).
	bool collected = false; // Czy kryszta³ zosta³ ju¿ zebrany (dotyczy tylko typu Crystal).
	bool useTexture = false; // Flaga okreœlaj¹ca, czy u¿ywaæ tekstury (sprite) czy kszta³tu (shape).
	float tileSize = 0.f; // Rozmiar kafelka w pikselach.

public:
	// Konstruktor: ustawia typ, pozycjê i rozmiar kafelka.
	Tile(TileType type, sf::Vector2f position, float size);

	// Rysuje kafelek na oknie, opcjonalnie z przesuniêciem (offset).
	void draw(sf::RenderWindow& window, sf::Vector2f offset = { 0.f, 0.f });
	TileType getType() const; // Zwraca typ kafelka.
	bool isCollected() const; // Zwraca status zebrania (dla kryszta³ów).
	void collect(); // Oznacza kafelek jako zebrany.
	sf::FloatRect getBounds() const; // Zwraca granice kafelka (do detekcji kolizji).
};