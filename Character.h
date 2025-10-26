#pragma once
#include <SFML/Graphics.hpp>

class Character
{
protected:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Vector2f position;
	float speed;

public:
	Character(const std::string& texturePath, sf::Vector2f startPosition, float speed);
	virtual ~Character() = default;

	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderWindow& window);

	sf::Vector2f getPosition() const;
	sf::FloatRect getBounds() const;
	void setPosition(sf::Vector2f newPosition);

};

