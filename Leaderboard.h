// Zapewnia, ¿e ten plik nag³ówkowy zostanie do³¹czony tylko raz.
#pragma once
// Standardowe nag³ówki dla stringów, wektorów i par.
#include <string>
#include <vector>
#include <utility>
#include <cstddef> // Dla std::size_t

// Klasa Leaderboard (Tabela Wyników) do zarz¹dzania wysokimi wynikami.
class Leaderboard {
public:
	// Konstruktor: maxEntries = maksymalna liczba wpisów (domyœlnie 10).
	explicit Leaderboard(std::size_t maxEntries = 10);

	// Wczytuje wyniki z pliku tekstowego (format: nazwa,wynik w ka¿dej linii).
	// Zwraca true w przypadku powodzenia.
	bool loadFromFile(const std::string& filename);
	// Zapisuje wyniki do pliku.
	bool saveToFile(const std::string& filename) const;

	// Dodaje nowy wynik. Lista jest automatycznie sortowana i przycinana do 'maxEntries'.
	void addScore(const std::string& name, int score);

	// Zwraca posortowan¹ listê wpisów (para: nazwa, wynik) malej¹co.
	std::vector<std::pair<std::string, int>> getEntries() const;

	
	void clear();// Czyœci ca³¹ listê wyników.

	// Zwraca true, jeœli podany wynik kwalifikuje siê do umieszczenia w rankingu (jest wystarczaj¹co wysoki).
	bool isHighScore(int score) const;

private:
	// Prywatna struktura przechowuj¹ca pojedynczy wpis (imiê i wynik).
	struct Entry { std::string name; int score; };
	std::vector<Entry> entries; // Wektor przechowuj¹cy aktualne wpisy.
	std::size_t maxEntries; // Maksymalna dopuszczalna liczba wpisów w rankingu.

	// Sortuje wpisy malej¹co i przycina wektor do 'maxEntries'.
	void sortAndClamp();
};