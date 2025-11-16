#pragma once
#include <string>
#include <vector>
#include <utility>
#include <cstddef>

class Leaderboard {
public:
	explicit Leaderboard(std::size_t maxEntries = 10); // Maksymalna liczba wpisów (domyœlnie 10).

	bool loadFromFile(const std::string& filename); // Wczytuje wyniki z pliku tekstowego (format: nazwa,wynik w ka¿dej linii).
	bool saveToFile(const std::string& filename) const; // Zapisuje wyniki do pliku.
	void addScore(const std::string& name, int score); // Dodaje nowy wynik. Lista jest automatycznie sortowana i przycinana do 'maxEntries'.
	std::vector<std::pair<std::string, int>> getEntries() const; // Zwraca posortowan¹ listê wpisów (para: nazwa, wynik) malej¹co.
	void clear();// Czyœci ca³¹ listê wyników.
	bool isHighScore(int score) const; // Zwraca true, jeœli podany wynik kwalifikuje siê do umieszczenia w rankingu (jest wystarczaj¹co wysoki).

private:
	struct Entry { std::string name; int score=0; }; // Struktura przechowuj¹ca pojedynczy wpis (imiê i wynik).
	std::vector<Entry> entries; // Wektor przechowuj¹cy aktualne wpisy.
	std::size_t maxEntries; // Maksymalna dopuszczalna liczba wpisów w rankingu.

	void sortAndClamp(); // Sortuje wpisy malej¹co i przycina wektor do 'maxEntries'.
};