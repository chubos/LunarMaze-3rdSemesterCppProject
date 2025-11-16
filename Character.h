#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Map;

class Character
{
protected:
	sf::Sprite sprite; // Obiekt do rysowania.
	sf::Texture texture; // Obraz postaci.
	sf::Vector2f position; // Pozycja w œwiecie.
	float speed; // Prêdkoœæ ruchu.
	sf::Vector2f spriteOffset{ 0.f, 0.f }; // Przesuniêcie dla wyœrodkowania duszka w kafelku.

public:
	
	Character(const std::string& texturePath, sf::Vector2f startPosition, float speed);
	virtual ~Character() = default;

	virtual void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) = 0; // Czysto wirtualna metoda aktualizacji stanu (logika ruchu/AI).
	virtual void draw(sf::RenderWindow& window, sf::Vector2f offset = { 0.f, 0.f }); // Wirtualna metoda rysowania.

	void scaleToTileSize(float tileSize); // Skaluje duszka do rozmiaru kafelka mapy.
	sf::Vector2f getPosition() const;
	sf::FloatRect getBounds() const; // Zwraca ramkê kolizyjn¹.
	void setPosition(sf::Vector2f newPosition);

};