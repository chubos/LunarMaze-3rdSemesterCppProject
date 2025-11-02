#pragma once
#include <SFML/Graphics.hpp>

class Map;

class Character
{
protected:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Vector2f position;
	float speed;
	sf::Vector2f spriteOffset{0.f, 0.f};

public:
	Character(const std::string& texturePath, sf::Vector2f startPosition, float speed);
	virtual ~Character() = default;

	virtual void update(float dt, Map* map = nullptr) = 0;
	virtual void draw(sf::RenderWindow& window);

	void scaleToTileSize(float tileSize);

	sf::Vector2f getPosition() const;
	sf::FloatRect getBounds() const;
	void setPosition(sf::Vector2f newPosition);

};

