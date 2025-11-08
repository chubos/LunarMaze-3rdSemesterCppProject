#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Map;

class Character
{
protected:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Vector2f position;
	float speed;
	// Offset to center the sprite inside a tile when using "contain" scaling
	sf::Vector2f spriteOffset{0.f, 0.f};

public:
	Character(const std::string& texturePath, sf::Vector2f startPosition, float speed);
	virtual ~Character() = default;

	// Optional playerPos allows AI characters to receive player location when updating
	virtual void update(float dt, Map* map = nullptr, std::optional<sf::Vector2f> playerPos = std::nullopt) = 0;
	virtual void draw(sf::RenderWindow& window, sf::Vector2f offset = {0.f, 0.f});

	// Scale sprite to fit map tile size (call after map is loaded)
	void scaleToTileSize(float tileSize);

	sf::Vector2f getPosition() const;
	sf::FloatRect getBounds() const;
	void setPosition(sf::Vector2f newPosition);

};

