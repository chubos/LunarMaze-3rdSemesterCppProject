#include "Map.h"
#include <fstream>
#include <iostream>

bool Map::loadFromFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
	std::cerr << "Nie mozna otworzyc mapy: " << filename << "\n";
		return false;
	}

	std::string line;
	int y = 0;
	tiles.clear();
	while (std::getline(file, line)) {
		width = static_cast<int>(line.size());
		for (int x = 0; x < width; x++) {
			char c = line[x];
			sf::Vector2f position(x * tileSize, y * tileSize);
			if (c == '#')
				tiles.emplace_back(TileType::Wall, position, tileSize);
			else if (c == '*')
				tiles.emplace_back(TileType::Crystal, position, tileSize);
			else
				tiles.emplace_back(TileType::Floor, position, tileSize);
		}
		y++;
	}
	height = y;
	return true;
}

void Map::draw(sf::RenderWindow& window) {
	for (auto& tile : tiles)
		tile.draw(window);
}

const std::vector<Tile>& Map::getTiles() const
{
	return tiles;
}

bool Map::isWallAt(float x, float y) const
{
	for (auto& tile : tiles) {
		if (tile.getType() == TileType::Wall && tile.getBounds().contains(x, y))
			return true;
	}
	return false;
}

bool Map::isWallCollision(sf::FloatRect bounds) const
{
	for (const auto& tile : tiles) {
		if (tile.getType() == TileType::Wall && tile.getBounds().intersects(bounds))
			return true;
	}
	return false;
}

bool Map::collectCrystalAt(sf::FloatRect bounds)
{
	for (auto& tile : tiles) {
		if (tile.getType() == TileType::Crystal && !tile.isCollected() && tile.getBounds().intersects(bounds)) {
			tile.collect();
			score += 10;
			return true;
		}
	}
	return false;
}

int Map::getScore() const
{
	return score;
}
