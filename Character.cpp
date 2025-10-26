#include "Character.h"
#include "Map.h"

Character::Character(const std::string& texturePath, sf::Vector2f startPosition, float speed)
	: position(startPosition), speed(speed)
{
	texture.loadFromFile(texturePath);
	sprite.setTexture(texture);
	sprite.setPosition(position);
}

void Character::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

sf::Vector2f Character::getPosition() const
{
	return position;
}

sf::FloatRect Character::getBounds() const
{
	return sprite.getGlobalBounds();
}

void Character::setPosition(sf::Vector2f newPosition)
{
	position = newPosition;
	sprite.setPosition(position);
}
