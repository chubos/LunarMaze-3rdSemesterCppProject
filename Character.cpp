#include "Character.h"
#include <algorithm>

Character::Character(const std::string& texturePath, sf::Vector2f startPosition, float speed)
	: position(startPosition), speed(speed)
{
	texture.loadFromFile(texturePath);
	sprite.setTexture(texture);
	sprite.setPosition(position);
}

void Character::scaleToTileSize(float tileSize)
{
	if (tileSize <= 0.f) return;

	auto texSize = texture.getSize();
	if (texSize.x == 0 || texSize.y == 0) return;

	const float marginFactor = 0.9f;
	float targetSize = tileSize * marginFactor;

	float scaleX = targetSize / static_cast<float>(texSize.x);
	float scaleY = targetSize / static_cast<float>(texSize.y);
	float scale = std::min(scaleX, scaleY);

	if (scale > 1.f) scale = 1.f;
	sprite.setScale(scale, scale);

	sprite.setOrigin(0.f, 0.f);

	float scaledW = static_cast<float>(texSize.x) * scale;
	float scaledH = static_cast<float>(texSize.y) * scale;
	spriteOffset.x = (tileSize - scaledW) * 0.5f;
	spriteOffset.y = (tileSize - scaledH) * 0.5f;

	sprite.setPosition(position + spriteOffset);
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
	sprite.setPosition(position + spriteOffset);
}
