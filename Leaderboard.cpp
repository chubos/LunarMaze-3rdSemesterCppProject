#include "Leaderboard.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>


Leaderboard::Leaderboard(std::size_t maxEntries)
    : maxEntries(maxEntries)
{
}


static inline void trimInPlace(std::string& s) { // Usuwa bia³e znaki (spacje, tabulatory, nowe linie) z pocz¹tku i koñca ci¹gu.
    const char* ws = " \t\r\n";
    auto a = s.find_first_not_of(ws); // ZnajdŸ pierwszy znak nie bêd¹cy bia³ym znakiem.
    if (a == std::string::npos) { s.clear(); return; } // Jeœli ca³y ci¹g to bia³e znaki, wyczyœæ go.
    auto b = s.find_last_not_of(ws); // ZnajdŸ ostatni znak nie bêd¹cy bia³ym znakiem.
    s = s.substr(a, b - a + 1); // Przytnij ci¹g do znalezionych granic.
}


bool Leaderboard::loadFromFile(const std::string& filename)
{
    entries.clear(); // Wyczyœæ obecne wpisy przed ³adowaniem.
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        auto pos = line.find_last_of(','); // ZnajdŸ ostatni przecinek (zak³adamy format: nazwa,wynik).
        if (pos == std::string::npos) continue; // Pomiñ linie bez przecinka.

        std::string name = line.substr(0, pos); // Nazwa to wszystko przed ostatnim przecinkiem.
        std::string scoreStr = line.substr(pos + 1); // Wynik to wszystko po ostatnim przecinku.

        trimInPlace(name);
        trimInPlace(scoreStr);

        if (name.empty() || scoreStr.empty()) continue; // Pomiñ, jeœli nazwa lub wynik s¹ puste.

        try {
            int score = std::stoi(scoreStr); // Konwersja ci¹gu na liczbê ca³kowit¹.
            entries.push_back({ name, score });
        }
        catch (...) {
            continue; // Pomiñ linie z niepoprawnym formatem wyniku.
        }
    }

    sortAndClamp(); // Posortuj i przytnij listê.
    return true;
}


bool Leaderboard::saveToFile(const std::string& filename) const
{ 
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    for (const auto& e : entries) {
        out << e.name << "," << e.score << '\n'; // Zapisz ka¿dy wpis w formacie: nazwa,wynik.
    }
    return true;
}


void Leaderboard::addScore(const std::string& name, int score)
{
    entries.push_back({ name, score });
    sortAndClamp();
}


std::vector<std::pair<std::string, int>> Leaderboard::getEntries() const
{
    std::vector<std::pair<std::string, int>> out;
    out.reserve(entries.size()); // Zarezerwuj pamiêæ, aby unikn¹æ realokacji.
    for (const auto& e : entries) out.emplace_back(e.name, e.score); // Skopiuj wpisy do wektora par.
    return out;
}


void Leaderboard::clear()
{
    entries.clear();
}


void Leaderboard::sortAndClamp()
{
    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
        if (a.score != b.score) return a.score > b.score; // G³ówny kryterium: sortowanie malej¹ce wed³ug wyniku.
        return a.name < b.name; // Drugorzêdne kryterium: sortowanie alfabetyczne rosn¹ce wed³ug nazwy (dla remisu).
        });

    if (entries.size() > maxEntries) entries.resize(maxEntries);
}


bool Leaderboard::isHighScore(int score) const
{
    if (score <= 0) return false;
    if (entries.empty()) return true;
	return score > entries.front().score; // Zwraca, czy wynik jest wy¿szy ni¿ najwy¿szy wynik na liœcie.
}