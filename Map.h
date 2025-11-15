#pragma once // Zapewnia, ¿e ten plik nag³ówkowy zostanie do³¹czony tylko raz.
// Do³¹czenie standardowych bibliotek SFML dla grafiki.
#include <SFML/Graphics.hpp>
// Do³¹czenie standardowych kontenerów.
#include <vector>
#include <string>
// Do³¹czenie nag³ówka klasy reprezentuj¹cej pojedynczy kafelek mapy.
#include "Tile.h"

// Klasa Map (Mapa) przechowuje i zarz¹dza siatk¹ kafelków oraz logik¹ zwi¹zan¹ z map¹.
class Map
{
	std::vector<Tile> tiles; // Wektor przechowuj¹cy wszystkie kafelki mapy (jednowymiarowy, logiczna siatka).
	int width = 0; // Szerokoœæ mapy w kafelkach.
	int height = 0; // Wysokoœæ mapy w kafelkach.
	float tileSize = 50.f; // Rozmiar pojedynczego kafelka w pikselach.
	int score = 0; // Aktualny wynik/liczba zebranych przedmiotów na mapie.


public:
	Map() = default; // Domyœlny konstruktor.
	// Wczytuje dane mapy (np. uk³ad kafelków) z pliku. Zwraca true w przypadku sukcesu.
	bool loadFromFile(const std::string& filename);
	// Rysuje mapê na oknie, opcjonalnie z przesuniêciem (offset).
	void draw(sf::RenderWindow& window, sf::Vector2f offset = { 0.f, 0.f });

	// Gettery.
	const std::vector<Tile>& getTiles() const; // Zwraca referencjê do wektora kafelków.
	// Sprawdza, czy w danej globalnej pozycji (x, y) znajduje siê œciana.
	bool isWallAt(float x, float y) const;

	// Sprawdza, czy podany prostok¹t kolizyjny (granice postaci) koliduje z jak¹kolwiek œcian¹ na mapie.
	bool isWallCollision(sf::FloatRect bounds) const;
	// Sprawdza, czy w danym obszarze znajduje siê kryszta³ do zebrania;
	// jeœli tak, zbiera go i zwraca true.
	bool collectCrystalAt(sf::FloatRect bounds);
	int getScore() const;

	// Zwraca rozmiar kafelka w pikselach (metoda inline).
	float getTileSize() const { return tileSize; }
	// Zwraca szerokoœæ mapy w kafelkach (metoda inline).
	int getWidth() const { return width; }
	// Zwraca wysokoœæ mapy w kafelkach (metoda inline).
	int getHeight() const { return height; }
};