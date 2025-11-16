#include "Map.h"
#include <fstream>
#include <iostream>

bool Map::loadFromFile(const std::string& filename) {
	std::ifstream file(filename); // Otwórz plik.
	if (!file.is_open()) { // Sprawdzenie, czy plik zosta³ otwarty.
		std::cerr << "Nie mozna otworzyc mapy: " << filename << "\n";
		return false; // Zwróæ b³¹d, jeœli plik jest niedostêpny.
	}

	score = 0; // Resetuj stan mapy, w tym wynik, podczas ³adowania nowej mapy.
	std::string line;
	int y = 0; // Licznik wierszy (wspó³rzêdna Y).
	tiles.clear(); // Wyczyœæ wektor kafelków.

	while (std::getline(file, line)) { // Czytaj plik linia po linii.
		width = static_cast<int>(line.size()); // Ustaw szerokoœæ mapy na podstawie d³ugoœci linii.
		for (int x = 0; x < width; x++) { // Iteruj przez znaki w linii (wspó³rzêdna X).
			char c = line[x];
			sf::Vector2f position(x * tileSize, y * tileSize); // Oblicz pozycjê w pikselach.

			// Tworzenie kafelka w zale¿noœci od znaku.
			if (c == '#')
				tiles.emplace_back(TileType::Wall, position, tileSize); // Œciana.
			else if (c == '*')
				tiles.emplace_back(TileType::Crystal, position, tileSize); // Kryszta³.
			else
				tiles.emplace_back(TileType::Floor, position, tileSize); // Pod³oga.
		}
		y++; // PrzejdŸ do nastêpnego wiersza.
	}
	height = y; // Ustaw wysokoœæ mapy.
	return true; // Pomyœlnie za³adowano.
}


void Map::draw(sf::RenderWindow& window, sf::Vector2f offset) { // Rysowanie mapy.
	sf::RenderStates states; // Stan renderowania.
	states.transform.translate(offset); // Zastosuj przesuniêcie mapy.

	
	for (auto& tile : tiles) // Wszyscy kafelki rysuj¹ siê same, otrzymuj¹c to samo przesuniêcie.
		tile.draw(window, offset);
}


const std::vector<Tile>& Map::getTiles() const // Pobieranie kafelków.
{
	return tiles; // Zwraca referencjê do wektora kafelków.
}


bool Map::isWallAt(float x, float y) const // Sprawdzenie, czy w danej pozycji znajduje siê œciana.
{
	for (auto& tile : tiles) {
		if (tile.getType() == TileType::Wall && tile.getBounds().contains(x, y)) // Czy kafelek to œciana i czy zawiera podane koordynaty.
			return true;
	}
	return false;
}


bool Map::isWallCollision(sf::FloatRect bounds) const // Sprawdzenie kolizji z dowoln¹ œcian¹.
{
	for (const auto& tile : tiles) {
		if (tile.getType() == TileType::Wall && tile.getBounds().intersects(bounds)) // Czy kafelek to œciana i czy jego granice przecinaj¹ siê z podanym prostok¹tem.
			return true;
	}
	return false;
}


bool Map::collectCrystalAt(sf::FloatRect bounds) // Zbieranie kryszta³u w danym obszarze
{
	for (auto& tile : tiles) {
		if (tile.getType() == TileType::Crystal && !tile.isCollected() && tile.getBounds().intersects(bounds)) { // Czy kafelek to kryszta³, czy nie zosta³ ju¿ zebrany, czy granice siê przecinaj¹.
			tile.collect(); // Oznacz kafelek jako zebrany.
			score += 10; // Dodaj punkty.
			return true;
		}
	}
	return false;
}

int Map::getScore() const
{
	return score; // Zwróæ aktualny wynik.
}