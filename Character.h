// Zapewnia, ¿e ten plik nag³ówkowy zostanie do³¹czony tylko raz.
#pragma once
#include <SFML/Graphics.hpp>
// Umo¿liwia u¿ycie wartoœci opcjonalnych.
#include <optional>

// ZapowiedŸ klasy Map.
class Map;

// Klasa bazowa dla postaci w grze.
class Character
{
protected:
	sf::Sprite sprite; // Obiekt do rysowania.
	sf::Texture texture; // Obraz postaci.
	sf::Vector2f position; // Pozycja w œwiecie.
	float speed; // Prêdkoœæ ruchu.
	// Przesuniêcie dla wyœrodkowania duszka w kafelku.
	sf::Vector2f spriteOffset{ 0.f, 0.f };

public:
	// Konstruktor.
	Character(const std::string& texturePath, sf::Vector2f startPosition, float speed);
	// Wirtualny destruktor.
	virtual ~Character() = default;

	// Czysto wirtualna metoda aktualizacji stanu (logika ruchu/AI).
	// dt = czas klatki; map = wskaŸnik do mapy; playerPos = opcjonalna pozycja gracza.
	virtual void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) = 0;
	// Wirtualna metoda rysowania.
	virtual void draw(sf::RenderWindow& window, sf::Vector2f offset = { 0.f, 0.f });

	// Skaluje duszka do rozmiaru kafelka mapy.
	void scaleToTileSize(float tileSize);

	sf::Vector2f getPosition() const;
	// Zwraca ramkê kolizyjn¹.
	sf::FloatRect getBounds() const;
	void setPosition(sf::Vector2f newPosition);

};