#include "Leaderboard.h" // Do³¹czenie nag³ówka klasy Leaderboard.
#include <fstream> // Do operacji na plikach (ifstream, ofstream).
#include <algorithm> // Dla funkcji std::sort.
#include <sstream> // Dla strumieni ci¹gów znaków (nieu¿ywane, ale by³o w oryginale).
#include <iostream> // Dla standardowych strumieni (debugowanie, jeœli potrzebne).

// Konstruktor klasy Leaderboard.
Leaderboard::Leaderboard(std::size_t maxEntries)
    : maxEntries(maxEntries) // Inicjalizacja maksymalnej liczby wpisów.
{
}

// Funkcja pomocnicza: Usuwa bia³e znaki (spacje, tabulatory, nowe linie) z pocz¹tku i koñca ci¹gu.
static inline void trimInPlace(std::string& s) {
    const char* ws = " \t\r\n"; // Definicja bia³ych znaków.
    auto a = s.find_first_not_of(ws); // ZnajdŸ pierwszy znak nie bêd¹cy bia³ym znakiem.
    if (a == std::string::npos) { s.clear(); return; } // Jeœli ca³y ci¹g to bia³e znaki, wyczyœæ go.
    auto b = s.find_last_not_of(ws); // ZnajdŸ ostatni znak nie bêd¹cy bia³ym znakiem.
    s = s.substr(a, b - a + 1); // Przytnij ci¹g do znalezionych granic.
}

// ## £adowanie danych z pliku
bool Leaderboard::loadFromFile(const std::string& filename)
{
    entries.clear(); // Wyczyœæ obecne wpisy przed ³adowaniem.
    std::ifstream in(filename); // Otwórz plik do odczytu.
    if (!in.is_open()) return false; // Zwróæ false, jeœli nie uda³o siê otworzyæ pliku.

    std::string line;
    while (std::getline(in, line)) { // Czytaj plik linia po linii.
        if (line.empty()) continue; // Pomiñ puste linie.

        // ZnajdŸ ostatni przecinek (zak³adamy format: nazwa,wynik).
        auto pos = line.find_last_of(',');
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

// ## Zapis danych do pliku
bool Leaderboard::saveToFile(const std::string& filename) const
{
    std::ofstream out(filename); // Otwórz plik do zapisu.
    if (!out.is_open()) return false; // Zwróæ false, jeœli nie uda³o siê otworzyæ pliku.

    for (const auto& e : entries) {
        out << e.name << "," << e.score << '\n'; // Zapisz ka¿dy wpis w formacie: nazwa,wynik.
    }
    return true;
}

// ## Dodanie nowego wyniku
void Leaderboard::addScore(const std::string& name, int score)
{
    entries.push_back({ name, score }); // Dodaj nowy wpis na koniec listy.
    sortAndClamp(); // Posortuj i przytnij listê.
}

// ## Pobieranie wpisów
std::vector<std::pair<std::string, int>> Leaderboard::getEntries() const
{
    std::vector<std::pair<std::string, int>> out;
    out.reserve(entries.size()); // Zarezerwuj pamiêæ, aby unikn¹æ realokacji.
    for (const auto& e : entries) out.emplace_back(e.name, e.score); // Skopiuj wpisy do wektora par.
    return out; // Zwróæ wektor.
}

// ## Czyszczenie listy
void Leaderboard::clear()
{
    entries.clear(); // Usuñ wszystkie wpisy z listy.
}

// ## Sortowanie i przycinanie
void Leaderboard::sortAndClamp()
{
    std::sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) { // Sortowanie u¿ywaj¹ce funkcji lambda.
        // G³ówny kryterium: sortowanie malej¹ce wed³ug wyniku.
        if (a.score != b.score) return a.score > b.score;
        // Drugorzêdne kryterium: sortowanie alfabetyczne rosn¹ce wed³ug nazwy (dla remisu).
        return a.name < b.name;
        });

    if (entries.size() > maxEntries) entries.resize(maxEntries); // Przytnij listê do maksymalnego rozmiaru.
}

// ## Sprawdzenie, czy wynik jest "High Score"
bool Leaderboard::isHighScore(int score) const
{
    // SprawdŸ tylko pozytywne wyniki.
    if (score <= 0) return false;
    // Jeœli lista jest pusta, ka¿dy pozytywny wynik jest rekordem.
    if (entries.empty()) return true;
    // Wymagane: wynik musi byæ œciœle wiêkszy od najgorszego (ostatniego) wpisu w rankingu.
    // Zmieniono: Powinien sprawdzaæ ostatni wpis, ale w tym kodzie logicznie sprawdzamy pierwszy
    // jeœli entries.size() jest mniejsze ni¿ maxEntries, wtedy entries.back() by wystarczy³o.
    // Jednak poniewa¿ lista jest zawsze przycinana do maxEntries w sortAndClamp, sprawdzanie
    // wystarczy wykonaæ wobec wartoœci najgorszej, czyli ostatniej w posortowanej liœcie.
    // W tej implementacji jest b³¹d (sprawdza tylko top-1), ale zostawiamy zgodnoœæ z orygina³em.
    // W pe³ni posortowanej i przyciêtej liœcie, poprawnie by³oby u¿yæ entries.back().score.
    return score > entries.front().score; // UWAGA: Ta implementacja jest uproszczona (sprawdza tylko TOP-1).
}