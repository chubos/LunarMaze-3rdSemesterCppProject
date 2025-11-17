#pragma once
#include <string>
#include <vector>
#include <utility>
#include <cstddef>

class Leaderboard {
public:
	explicit Leaderboard(std::size_t maxEntries = 10); // Maksymalna liczba wpisów (domyœlnie 10).

	bool loadFromFile(const std::string& filename);
	bool saveToFile(const std::string& filename) const;
	void addScore(const std::string& name, int score);
	std::vector<std::pair<std::string, int>> getEntries() const;
	void clear();
	bool isHighScore(int score) const;

private:
	struct Entry { std::string name; int score = 0; };
	std::vector<Entry> entries;
	std::size_t maxEntries;

	void sortAndClamp();
};