#include "Map.h" // Do³¹czenie nag³ówka klasy Map.
#include <fstream> // Do operacji na plikach (std::ifstream).
#include <iostream> // Do operacji wejœcia/wyjœcia (std::cerr).

// ## £adowanie mapy z pliku tekstowego
bool Map::loadFromFile(const std::string& filename) {
	std::ifstream file(filename); // Otwórz plik.
	if (!file.is_open()) { // Sprawdzenie, czy plik zosta³ otwarty.
		std::cerr << "Nie mozna otworzyc mapy: " << filename << "\n";
		return false; // Zwróæ b³¹d, jeœli plik jest niedostêpny.
	}

	// Resetuj stan mapy, w tym wynik, podczas ³adowania nowej mapy.
	score = 0;
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

// ## Rysowanie mapy
void Map::draw(sf::RenderWindow& window, sf::Vector2f offset) {
	sf::RenderStates states; // Stan renderowania.
	states.transform.translate(offset); // Zastosuj przesuniêcie kamery/mapy.

	// Wszyscy kafelki rysuj¹ siê sami, otrzymuj¹c to samo przesuniêcie.
	for (auto& tile : tiles)
		tile.draw(window, offset);
}

// ## Pobieranie kafelków
const std::vector<Tile>& Map::getTiles() const
{
	return tiles; // Zwraca referencjê do wektora kafelków.
}

// ## Sprawdzenie, czy w danej pozycji znajduje siê œciana
bool Map::isWallAt(float x, float y) const
{
	for (auto& tile : tiles) {
		// SprawdŸ, czy kafelek to œciana i czy zawiera podane koordynaty.
		if (tile.getType() == TileType::Wall && tile.getBounds().contains(x, y))
			return true;
	}
	return false;
}

// ## Sprawdzenie kolizji z dowoln¹ œcian¹
bool Map::isWallCollision(sf::FloatRect bounds) const
{
	for (const auto& tile : tiles) {
		// SprawdŸ, czy kafelek to œciana i czy jego granice przecinaj¹ siê z podanym prostok¹tem.
		if (tile.getType() == TileType::Wall && tile.getBounds().intersects(bounds))
			return true;
	}
	return false;
}

// ## Zbieranie kryszta³u w danym obszarze
bool Map::collectCrystalAt(sf::FloatRect bounds)
{
	for (auto& tile : tiles) {
		// 1. Czy kafelek to kryszta³?
		// 2. Czy nie zosta³ ju¿ zebrany?
		// 3. Czy granice siê przecinaj¹?
		if (tile.getType() == TileType::Crystal && !tile.isCollected() && tile.getBounds().intersects(bounds)) {
			tile.collect(); // Oznacz kafelek jako zebrany.
			score += 10; // Dodaj punkty.
			return true; // PotwierdŸ, ¿e coœ zosta³o zebrane.
		}
	}
	return false; // Nic nie zosta³o zebrane.
}

// ## Pobieranie aktualnego wyniku
int Map::getScore() const
{
	return score; // Zwróæ aktualny wynik.
}