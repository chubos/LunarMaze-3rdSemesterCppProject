#include "Tile.h"
#include <iostream>

namespace {
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
		wallTexOk = wallTexture.loadFromFile("assets/wall.png");
		if (!wallTexOk) std::cerr << "Failed to load assets/wall.png\n";
		floorTexOk = floorTexture.loadFromFile("assets/floor.png");
		if (!floorTexOk) std::cerr << "Failed to load assets/floor.png\n";
		crystalTexOk = crystalTexture.loadFromFile("assets/crystal.png");
		if (!crystalTexOk) std::cerr << "Failed to load assets/crystal.png\n";
		texturesLoaded = true;
	}
}

Tile::Tile(TileType type, sf::Vector2f position, float size) 
	: type(type), tileSize(size)
{
	shape.setSize({ size, size });
	shape.setPosition(position);

	// default colors
	if (type == TileType::Wall)
		shape.setFillColor(sf::Color(60, 60, 80));
	else if (type == TileType::Floor)
		shape.setFillColor(sf::Color(20, 20, 40));
	else if (type == TileType::Crystal)
		shape.setFillColor(sf::Color(0, 200, 255));

	// try to load textures (once)
	ensureTileTexturesLoaded();

	// select texture if available
	if (type == TileType::Wall && wallTexOk) {
		useTexture = true;
		// reset texture rect to full texture
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
		} else {
			float sx = size / static_cast<float>(texSize.x);
			float sy = size / static_cast<float>(texSize.y);
			sprite.setScale(sx, sy);
			sprite.setOrigin(0.f, 0.f);
			sprite.setPosition(position);
		}
	}
}

void Tile::draw(sf::RenderWindow& window, sf::Vector2f offset) {
	// draw sprite (with offset) if texture is used, otherwise draw colored shape
	if (useTexture) {
		sf::RenderStates states;
		states.transform.translate(offset);
		window.draw(sprite, states);
	}
	else {
		sf::RenderStates states;
		states.transform.translate(offset);
		window.draw(shape, states);
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
	// mark collected
	collected = true;
	// replace crystal with floor visually and logically
	if (type == TileType::Crystal) {
		type = TileType::Floor;
		// if we have floor texture, switch sprite to floor and reset rect/scale/position
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
		} else {
			useTexture = false;
			shape.setFillColor(sf::Color(20, 20, 40));
		}
	}
}

sf::FloatRect Tile::getBounds() const {
	if (useTexture) return sprite.getGlobalBounds();
	return shape.getGlobalBounds();
}