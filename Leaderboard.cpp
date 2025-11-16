#include "Leaderboard.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>


Leaderboard::Leaderboard(std::size_t maxEntries)
    : maxEntries(maxEntries) // Inicjalizacja maksymalnej liczby wpisów.
{
}


static inline void trimInPlace(std::string& s) { // Funkcja pomocnicza: Usuwa bia³e znaki (spacje, tabulatory, nowe linie) z pocz¹tku i koñca ci¹gu.
    const char* ws = " \t\r\n"; // Definicja bia³ych znaków.
    auto a = s.find_first_not_of(ws); // ZnajdŸ pierwszy znak nie bêd¹cy bia³ym znakiem.
    if (a == std::string::npos) { s.clear(); return; } // Jeœli ca³y ci¹g to bia³e znaki, wyczyœæ go.
    auto b = s.find_last_not_of(ws); // ZnajdŸ ostatni znak nie bêd¹cy bia³ym znakiem.
    s = s.substr(a, b - a + 1); // Przytnij ci¹g do znalezionych granic.
}


bool Leaderboard::loadFromFile(const std::string& filename) // £adowanie danych z pliku
{
    entries.clear(); // Wyczyœæ obecne wpisy przed ³adowaniem.
    std::ifstream in(filename); // Otwórz plik do odczytu.
    if (!in.is_open()) return false; // Zwróæ false, jeœli nie uda³o siê otworzyæ pliku.

    std::string line;
    while (std::getline(in, line)) { // Czytaj plik linia po linii.
        if (line.empty()) continue; // Pomiñ puste linie.

        auto pos = line.find_last_of(','); // ZnajdŸ ostatni przecinek (zak³adamy format: nazwa,wynik).
        if (pos == std::string::npos) continue; // Pomiñ linie bez przecinka.

        std::string name = line.substr(0, pos); // Nazwa to wszystko przed ostatnim przecinkiem.
        std::string scoreStr = line.substr(pos + 1); // Wynik to wszystko po ostatnim przecinku.

        trimInPlace(name); // Usuñ bia³e znaki z nazwy.
        trimInPlace(scoreStr); // Usuñ bia³e znaki z ci¹gu wyniku.

        if (name.empty() || scoreStr.empty()) continue; // Pomiñ, jeœli nazwa lub wynik s¹ puste.

        try {
            int score = std::stoi(scoreStr); // Konwersja ci¹gu na liczbê ca³kowit¹.
            entries.push_back({ name, score }); // Dodanie nowego wpisu.
        }
        catch (...) {
            continue; // Pomiñ linie z niepoprawnym formatem wyniku.
        }
    }

    sortAndClamp(); // Posortuj i przytnij listê do maksymalnego rozmiaru.
    return true;
}


bool Leaderboard::saveToFile(const std::string& filename) const // Zapis danych do pliku
{ 
    std::ofstream out(filename); // Otwórz plik do zapisu.
    if (!out.is_open()) return false; // Zwróæ false, jeœli nie uda³o siê otworzyæ pliku.

    for (const auto& e : entries) {
        out << e.name << "," << e.score << '\n'; // Zapisz ka¿dy wpis w formacie: nazwa,wynik.
    }
    return true;
}


void Leaderboard::addScore(const std::string& name, int score) // Dodanie nowego wyniku
{
    entries.push_back({ name, score }); // Dodaj nowy wpis na koniec listy.
    sortAndClamp(); // Posortuj i przytnij listê.
}


std::vector<std::pair<std::string, int>> Leaderboard::getEntries() const // Pobieranie wpisów
{
    std::vector<std::pair<std::string, int>> out;
    out.reserve(entries.size()); // Zarezerwuj pamiêæ, aby unikn¹æ realokacji.
    for (const auto& e : entries) out.emplace_back(e.name, e.score); // Skopiuj wpisy do wektora par.
    return out; // Zwróæ wektor.
}


void Leaderboard::clear() // Czyszczenie listy
{
    entries.clear(); // Usuñ wszystkie wpisy z listy.
}


void Leaderboard::sortAndClamp() // Sortowanie i przycinanie
{
    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) { // Sortowanie u¿ywaj¹ce funkcji lambda.
        if (a.score != b.score) return a.score > b.score; // G³ówny kryterium: sortowanie malej¹ce wed³ug wyniku.
        return a.name < b.name; // Drugorzêdne kryterium: sortowanie alfabetyczne rosn¹ce wed³ug nazwy (dla remisu).
        });

    if (entries.size() > maxEntries) entries.resize(maxEntries); // Przytnij listê do maksymalnego rozmiaru.
}


bool Leaderboard::isHighScore(int score) const // Sprawdzenie, czy wynik jest "High Score"
{
    if (score <= 0) return false; // SprawdŸ tylko pozytywne wyniki.
    if (entries.empty()) return true; // Jeœli lista jest pusta, ka¿dy pozytywny wynik jest rekordem.
	return score > entries.front().score; // Zwraca, czy wynik jest wy¿szy ni¿ najwy¿szy wynik na liœcie.
}